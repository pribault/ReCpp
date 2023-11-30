#pragma once

#include <recpp/subscriptions/RangeSubscription.h>

template <typename T, class I>
recpp::publishers::RangePublisher<T, I>::RangePublisher(I first, I last)
	: m_first(first)
	, m_last(last)
{
}

template <typename T, class I>
void recpp::publishers::RangePublisher<T, I>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	auto subscription = recpp::subscriptions::RangeSubscription<T, I>(subscriber, m_first, m_last);
	subscriber.onSubscribe(subscription);
}
