#pragma once

#include <recpp/subscribers/DefaultSubscriber.h>

#include <algorithm>
#include <iterator>

template <typename T, typename P>
recpp::subscriptions::MergeSubscription<T, P>::MergeSubscription(const rscpp::Subscriber<T> &subscriber, rscpp::Publisher<P> &publisherSource,
																 const std::optional<std::reference_wrapper<recpp::async::Scheduler>> &scheduler)
	: rscpp::Subscription(std::make_shared<Impl>(subscriber, publisherSource, scheduler))
{
}

template <typename T, typename P>
recpp::subscriptions::MergeSubscription<T, P>::Impl::Impl(const rscpp::Subscriber<T> &subscriber, rscpp::Publisher<P> &publisherSource,
														  const std::optional<std::reference_wrapper<recpp::async::Scheduler>> &scheduler)
	: m_subscriber(subscriber)
{
	auto sourceSubscriber = recpp::subscribers::DefaultSubscriber<P>(
		[this, scheduler](P publisher)
		{
			m_remaining++;
			const auto publisherId = m_currentId++;
			auto	   subscriber = recpp::subscribers::DefaultSubscriber<T>(
				  [this, scheduler](const T &value)
				  {
					  if (scheduler)
						  scheduler->get().schedule(recpp::async::Schedulable(
							  [this, value]()
							  {
								  onPublisherNextValue(value);
							  }));
					  else
						  onPublisherNextValue(value);
				  },
				  [this, scheduler, publisherId](const std::exception_ptr &exceptionPtr)
				  {
					  if (scheduler)
						  scheduler->get().schedule(recpp::async::Schedulable(
							  [this, publisherId, exceptionPtr]()
							  {
								  onPublisherError(publisherId, exceptionPtr);
							  }));
					  else
						  onPublisherError(publisherId, exceptionPtr);
				  },
				  [this, scheduler, publisherId]()
				  {
					  if (scheduler)
						  scheduler->get().schedule(recpp::async::Schedulable(
							  [this, publisherId]()
							  {
								  onPublisherComplete(publisherId);
							  }));
					  else
						  onPublisherComplete(publisherId);
				  },
				  [this, scheduler, publisherId](rscpp::Subscription &subscription)
				  {
					  if (scheduler)
						  scheduler->get().schedule(recpp::async::Schedulable(
							  [this, publisherId, subscription]()
							  {
								  m_subscriptions.emplace_back(publisherId, subscription);
								  tryRequest();
							  }));
					  else
					  {
						  m_subscriptions.emplace_back(publisherId, subscription);
						  tryRequest();
					  }
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
			sourceCompleted = true;
			if (!m_completed && !m_remaining)
			{
				m_completed = true;
				if (!m_canceled)
					m_subscriber.onComplete();
			}
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
	if (!m_canceled)
		m_subscriber.onNext(value);
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
	if (!m_completed && !m_remaining && sourceCompleted)
	{
		m_completed = true;
		if (!m_canceled)
			m_subscriber.onComplete();
	}
}

template <typename T, typename P>
void recpp::subscriptions::MergeSubscription<T, P>::Impl::removePublisher(size_t publisherId)
{
	const auto it = std::find_if(std::begin(m_subscriptions), std::end(m_subscriptions),
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
	while (m_requested)
	{
		if (m_subscriptions.empty() || !m_remaining || m_canceled)
			break;

		m_requested--;
		auto subscription = m_subscriptions[m_currentIndex].second;
		subscription.request(1);
		if (!m_subscriptions.empty())
			m_currentIndex = (m_currentIndex + 1) % m_subscriptions.size();
	}
}
