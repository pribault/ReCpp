#pragma once

#include <recpp/async/Schedulable.h>
#include <recpp/async/Scheduler.h>
#include <recpp/subscriptions/ForwardSubscription.h>

template <typename T>
recpp::SubscribeOn<T>::SubscribeOn(const rscpp::Publisher<T> &publisher, recpp::async::Scheduler &scheduler)
	: rscpp::Processor<T, T>(std::shared_ptr<rscpp::Processor<T, T>>(new Impl(*this, publisher, scheduler)))
{
}

template <typename T>
recpp::SubscribeOn<T>::Impl::Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, recpp::async::Scheduler &scheduler)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_scheduler(scheduler)
{
}

template <typename T>
void recpp::SubscribeOn<T>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	auto forwardSubscription = ForwardSubscription(subscription);
	m_subscriber.onSubscribe(forwardSubscription);
}

template <typename T>
void recpp::SubscribeOn<T>::Impl::onNext(const T &value)
{
	m_subscriber.onNext(value);
}

template <typename T>
void recpp::SubscribeOn<T>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
}

template <typename T>
void recpp::SubscribeOn<T>::Impl::onComplete()
{
	m_subscriber.onComplete();
}

template <typename T>
void recpp::SubscribeOn<T>::Impl::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_subscriber = subscriber;
	auto publisher = m_publisher;
	auto parent = m_parent;
	m_scheduler.schedule(async::Schedulable([publisher, parent]() mutable { publisher.subscribe(parent); }));
}
