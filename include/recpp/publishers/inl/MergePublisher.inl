#pragma once

#include <recpp/subscriptions/MergeSubscription.h>

template <typename T, typename P>
recpp::publishers::MergePublisher<T, P>::MergePublisher(rscpp::Publisher<P> &publisherSource)
	: m_publisherSource(publisherSource)
{
}

template <typename T, typename P>
void recpp::publishers::MergePublisher<T, P>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	auto subscription = recpp::subscriptions::MergeSubscription<T, P>(subscriber, m_publisherSource);
	subscriber.onSubscribe(subscription);
}
