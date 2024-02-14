#pragma once

#include <recpp/subscriptions/ForwardSubscription.h>

template <typename T, typename Rep, typename Period>
recpp::processors::Delay<T, Rep, Period>::Delay(const rscpp::Publisher<T> &publisher, recpp::async::Scheduler &scheduler,
												const std::chrono::duration<Rep, Period> &delay, bool delayError)
	: rscpp::Processor<T, T>(std::make_shared<Impl>(*this, publisher, scheduler, delay, delayError))
{
}

template <typename T, typename Rep, typename Period>
recpp::processors::Delay<T, Rep, Period>::Impl::Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, recpp::async::Scheduler &scheduler,
													 const std::chrono::duration<Rep, Period> &delay, bool delayError)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_scheduler(scheduler)
	, m_delayDuration(delay)
	, m_delayError(delayError)
{
}

template <typename T, typename Rep, typename Period>
void recpp::processors::Delay<T, Rep, Period>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	auto forwardSubscription = recpp::subscriptions::ForwardSubscription(subscription);
	m_subscriber.onSubscribe(forwardSubscription);
}

template <typename T, typename Rep, typename Period>
void recpp::processors::Delay<T, Rep, Period>::Impl::onNext(const T &value)
{
	auto subscriber = m_subscriber;
	m_scheduler.schedule(m_delayDuration, recpp::async::Schedulable([subscriber, value]() mutable { subscriber.onNext(value); }));
}

template <typename T, typename Rep, typename Period>
void recpp::processors::Delay<T, Rep, Period>::Impl::onError(const std::exception_ptr &error)
{
	auto subscriber = m_subscriber;
	m_scheduler.schedule(m_delayDuration, recpp::async::Schedulable([subscriber, error]() mutable { subscriber.onError(error); }));
}

template <typename T, typename Rep, typename Period>
void recpp::processors::Delay<T, Rep, Period>::Impl::onComplete()
{
	auto subscriber = m_subscriber;
	m_scheduler.schedule(m_delayDuration, recpp::async::Schedulable([subscriber]() mutable { subscriber.onComplete(); }));
}

template <typename T, typename Rep, typename Period>
void recpp::processors::Delay<T, Rep, Period>::Impl::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
