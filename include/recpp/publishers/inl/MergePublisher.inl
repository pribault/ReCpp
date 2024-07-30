#pragma once

#include <recpp/subscriptions/MergeSubscription.h>

template <typename T, typename P>
recpp::publishers::MergePublisher<T, P>::MergePublisher(rscpp::Publisher<P>													 &publisherSource,
														const std::optional<std::reference_wrapper<recpp::async::Scheduler>> &scheduler)
	: m_publisherSource(publisherSource)
	, m_scheduler(scheduler)
{
}

template <typename T, typename P>
void recpp::publishers::MergePublisher<T, P>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	auto subscription = recpp::subscriptions::MergeSubscription<T, P>(subscriber, m_publisherSource, m_scheduler);
	subscriber.onSubscribe(subscription);
}
