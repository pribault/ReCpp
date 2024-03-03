#pragma once

#include <recpp/subscribers/DefaultSubscriber.h>

#include <algorithm>
#include <iterator>

template <typename T, typename P>
recpp::subscriptions::MergeSubscription<T, P>::MergeSubscription(const rscpp::Subscriber<T> &subscriber, rscpp::Publisher<P> &publisherSource)
	: rscpp::Subscription(std::make_shared<Impl>(subscriber, publisherSource))
{
}

template <typename T, typename P>
recpp::subscriptions::MergeSubscription<T, P>::Impl::Impl(const rscpp::Subscriber<T> &subscriber, rscpp::Publisher<P> &publisherSource)
	: m_subscriber(subscriber)
{
	publisherSource.subscribe(recpp::subscribers::DefaultSubscriber<P>(
		[this](P publisher)
		{
			m_remaining++;
			const auto publisherId = m_currentId++;
			publisher.Publisher<T>::subscribe(recpp::subscribers::DefaultSubscriber<T>(
				[this](const T &value)
				{
					onPublisherNextValue(value);
				},
				[this, publisherId](const std::exception_ptr &exceptionPtr)
				{
					onPublisherError(publisherId, exceptionPtr);
				},
				[this, publisherId]()
				{
					onPublisherComplete(publisherId);
				},
				[this, publisherId](rscpp::Subscription &subscription)
				{
					m_subscriptions[publisherId] = subscription;
				}));
		},
		[this](const std::exception_ptr &exceptionPtr)
		{
			cancel();
			m_subscriber.onError(exceptionPtr);
		},
		[this]()
		{
			sourceCompleted = true;
			if (!m_remaining)
			{
				m_completed = true;
				if (!m_canceled)
					m_subscriber.onComplete();
			}
		}));
}

template <typename T, typename P>
void recpp::subscriptions::MergeSubscription<T, P>::Impl::request(size_t count)
{
	/*for (size_t i = 0; i < count; i++)
	{
		if (!m_remaining || m_canceled)
			return;

		m_subscriptions[m_currentIndex++].request(count);
	}*/
}

template <typename T, typename P>
void recpp::subscriptions::MergeSubscription<T, P>::Impl::cancel()
{
	m_canceled = true;
	for (auto &[publisher, subscription] : m_subscriptions)
		subscription.cancel();
}

template <typename T, typename P>
void recpp::subscriptions::MergeSubscription<T, P>::Impl::onPublisherNextValue(const T &value)
{
	if (!m_canceled)
		m_subscriber.onNext(value);
}

template <typename T, typename P>
void recpp::subscriptions::MergeSubscription<T, P>::Impl::onPublisherError(size_t publisherId, const std::exception_ptr &exceptionPtr)
{
	const auto it = m_subscriptions.find(publisherId);
	if (it != std::end(m_subscriptions))
	{
		const auto index = std::distance(std::begin(m_subscriptions), it);
		if (m_currentIndex > index)
			m_currentIndex--;
		m_subscriptions.erase(it);
	}
	if (m_remaining)
		m_remaining--;
	cancel();
	m_subscriber.onError(exceptionPtr);
}

template <typename T, typename P>
void recpp::subscriptions::MergeSubscription<T, P>::Impl::onPublisherComplete(size_t publisherId)
{
	const auto it = m_subscriptions.find(publisherId);
	if (it != std::end(m_subscriptions))
	{
		const auto index = std::distance(std::begin(m_subscriptions), it);
		if (m_currentIndex > index)
			m_currentIndex--;
		m_subscriptions.erase(it);
	}
	if (m_remaining)
		m_remaining--;
	if (!m_remaining && sourceCompleted)
	{
		m_completed = true;
		if (!m_canceled)
			m_subscriber.onComplete();
	}
}
