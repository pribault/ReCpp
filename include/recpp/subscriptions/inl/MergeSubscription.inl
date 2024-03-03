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
	auto sourceSubscriber = recpp::subscribers::DefaultSubscriber<P>(
		[this](P publisher)
		{
			m_remaining++;
			const auto publisherId = m_currentId++;
			auto	   subscriber = recpp::subscribers::DefaultSubscriber<T>(
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
					  m_mutex.lock();
					  m_subscriptions.emplace_back(publisherId, subscription);
					  m_mutex.unlock();
					  tryRequest();
				  },
				  false);
			publisher.rscpp::template Publisher<T>::subscribe(subscriber);
		},
		[this](const std::exception_ptr &exceptionPtr)
		{
			cancel();
			m_subscriber.onError(exceptionPtr);
		},
		[this]()
		{
			m_mutex.lock();
			sourceCompleted = true;
			if (!m_completed && !m_remaining)
			{
				m_completed = true;
				if (!m_canceled)
				{
					m_mutex.unlock();
					m_subscriber.onComplete();
				}
				else
					m_mutex.unlock();
			}
			else
				m_mutex.unlock();
		});
	publisherSource.subscribe(sourceSubscriber);
}

template <typename T, typename P>
void recpp::subscriptions::MergeSubscription<T, P>::Impl::request(size_t count)
{
	m_requested += count;
	tryRequest();
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
	m_mutex.lock();
	if (!m_canceled)
	{
		m_mutex.unlock();
		m_subscriber.onNext(value);
	}
	else
		m_mutex.unlock();
}

template <typename T, typename P>
void recpp::subscriptions::MergeSubscription<T, P>::Impl::onPublisherError(size_t publisherId, const std::exception_ptr &exceptionPtr)
{
	removePublisher(publisherId);
	cancel();
	m_subscriber.onError(exceptionPtr);
}

template <typename T, typename P>
void recpp::subscriptions::MergeSubscription<T, P>::Impl::onPublisherComplete(size_t publisherId)
{
	removePublisher(publisherId);
	m_mutex.lock();
	if (!m_completed && !m_remaining && sourceCompleted)
	{
		m_completed = true;
		if (!m_canceled)
		{
			m_mutex.unlock();
			m_subscriber.onComplete();
		}
		else
			m_mutex.unlock();
	}
	else
		m_mutex.unlock();
}

template <typename T, typename P>
void recpp::subscriptions::MergeSubscription<T, P>::Impl::removePublisher(size_t publisherId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	const auto					it = std::find_if(std::begin(m_subscriptions), std::end(m_subscriptions),
												  [publisherId](const auto &pair)
												  {
									  return pair.first == publisherId;
								  });
	if (it != std::end(m_subscriptions))
	{
		const auto index = std::distance(std::begin(m_subscriptions), it);
		if (m_currentIndex > index)
			m_currentIndex--;
		else if (m_currentIndex == m_subscriptions.size() - 1)
			m_currentIndex = 0;
		m_subscriptions.erase(it);
	}
	if (m_remaining)
		m_remaining--;
}

template <typename T, typename P>
void recpp::subscriptions::MergeSubscription<T, P>::Impl::tryRequest()
{
	m_mutex.lock();
	while (m_requested)
	{
		if (m_subscriptions.empty() || !m_remaining || m_canceled)
		{
			m_mutex.unlock();
			return;
		}

		m_requested--;
		auto subscription = m_subscriptions[m_currentIndex].second;
		m_mutex.unlock();
		subscription.request(1);
		m_mutex.lock();
		if (!m_subscriptions.empty())
			m_currentIndex = (m_currentIndex + 1) % m_subscriptions.size();
	}
	m_mutex.unlock();
}
