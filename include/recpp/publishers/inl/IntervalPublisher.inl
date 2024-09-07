#pragma once

#include <recpp/subscriptions/IntervalSubscription.h>

template <typename T>
recpp::publishers::IntervalPublisher<T>::IntervalPublisher(const recpp::async::Scheduler::TimePoint &start, const recpp::async::Scheduler::Duration &period,
														   recpp::async::Scheduler &scheduler)
	: m_start(start)
	, m_period(period)
	, m_scheduler(scheduler)
{
}

template <typename T>
void recpp::publishers::IntervalPublisher<T>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	auto subscription = recpp::subscriptions::IntervalSubscription<T>(subscriber, m_start, m_period, m_scheduler);
	subscriber.onSubscribe(subscription);
}
