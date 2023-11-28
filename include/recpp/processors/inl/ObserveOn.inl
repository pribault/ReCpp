#pragma once

#include <recpp/async/Schedulable.h>
#include <recpp/async/Scheduler.h>
#include <recpp/subscriptions/ForwardSubscription.h>

template <typename T>
recpp::ObserveOn<T>::ObserveOn(const rscpp::Publisher<T> &publisher, recpp::async::Scheduler &scheduler)
	: rscpp::Processor<T, T>(std::shared_ptr<rscpp::Processor<T, T>>(new Impl(*this, publisher, scheduler)))
{
}

template <typename T>
recpp::ObserveOn<T>::Impl::Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, recpp::async::Scheduler &scheduler)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_scheduler(scheduler)
{
}

template <typename T>
void recpp::ObserveOn<T>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	auto forwardSubscription = ForwardSubscription(subscription);
	m_subscriber.onSubscribe(forwardSubscription);
}

template <typename T>
void recpp::ObserveOn<T>::Impl::onNext(const T &value)
{
	auto subscriber = m_subscriber;
	m_scheduler.schedule(Schedulable([subscriber, value]() mutable { subscriber.onNext(value); }));
}

template <typename T>
void recpp::ObserveOn<T>::Impl::onError(const std::exception_ptr &error)
{
	auto subscriber = m_subscriber;
	m_scheduler.schedule(Schedulable([subscriber, error]() mutable { subscriber.onError(error); }));
}

template <typename T>
void recpp::ObserveOn<T>::Impl::onComplete()
{
	auto subscriber = m_subscriber;
	m_scheduler.schedule(Schedulable([subscriber]() mutable { subscriber.onComplete(); }));
}

template <typename T>
void recpp::ObserveOn<T>::Impl::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
