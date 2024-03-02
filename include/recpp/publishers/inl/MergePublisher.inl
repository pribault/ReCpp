#pragma once

#include <recpp/subscriptions/MergeSubscription.h>

template <typename T, class I>
recpp::publishers::MergePublisher<T, I>::MergePublisher(I first, I last)
	: m_first(first)
	, m_last(last)
{
}

template <typename T, class I>
void recpp::publishers::MergePublisher<T, I>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	auto subscription = recpp::subscriptions::MergeSubscription<T, I>(subscriber, m_first, m_last);
	subscriber.onSubscribe(subscription);
}
