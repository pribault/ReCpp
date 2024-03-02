#pragma once

#include <recpp/subscribers/DefaultSubscriber.h>

#include <algorithm>
#include <iterator>

template <typename T, class I>
recpp::subscriptions::MergeSubscription<T, I>::MergeSubscription(const rscpp::Subscriber<T> &subscriber, I first, I last)
	: rscpp::Subscription(std::make_shared<Impl>(subscriber, first, last))
{
}

template <typename T, class I>
recpp::subscriptions::MergeSubscription<T, I>::Impl::Impl(const rscpp::Subscriber<T> &subscriber, I first, I last)
	: m_subscriber(subscriber)
{
	m_remaining = std::distance(first, last);
	size_t index = 0;
	for (auto it = first; it != last; it++)
	{
		auto &publisher = *it;
		index++;
		publisher.Publisher<T>::subscribe(recpp::subscribers::DefaultSubscriber<T>(
			[this](const T &value)
			{
				onPublisherNextValue(value);
			},
			[this, index](const std::exception_ptr &exceptionPtr)
			{
				onPublisherError(index, exceptionPtr);
			},
			[this, index]()
			{
				onPublisherComplete(index);
			},
			[this, index](rscpp::Subscription &subscription)
			{
				m_subscriptions[index] = subscription;
			}));
	};
}

template <typename T, class I>
void recpp::subscriptions::MergeSubscription<T, I>::Impl::request(size_t count)
{
	for (size_t i = 0; i < count; i++)
	{
		if (!m_remaining || m_canceled)
			return;

		m_subscriptions[m_currentIndex++].request(count);
	}
}

template <typename T, class I>
void recpp::subscriptions::MergeSubscription<T, I>::Impl::cancel()
{
	m_canceled = true;
	for (auto &[publisher, subscription] : m_subscriptions)
		subscription.cancel();
}

template <typename T, class I>
void recpp::subscriptions::MergeSubscription<T, I>::Impl::onPublisherNextValue(const T &value)
{
	if (!m_canceled)
		m_subscriber.onNext(value);
}

template <typename T, class I>
void recpp::subscriptions::MergeSubscription<T, I>::Impl::onPublisherError(size_t publisherId, const std::exception_ptr &exceptionPtr)
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

template <typename T, class I>
void recpp::subscriptions::MergeSubscription<T, I>::Impl::onPublisherComplete(size_t publisherId)
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
	if (!m_remaining)
	{
		m_completed = true;
		if (!m_canceled)
			m_subscriber.onComplete();
	}
}
