#pragma once

#include <recpp/subscriptions/ForwardSubscription.h>

template <typename T, typename R>
recpp::processors::Reduce<T, R>::Reduce(const rscpp::Publisher<T> &publisher, T init, R operation)
	: rscpp::Processor<T, T>(std::make_shared<Impl>(*this, publisher, init, operation))
{
}

template <typename T, typename R>
recpp::processors::Reduce<T, R>::Impl::Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, T init, R operation)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_current(init)
	, m_operation(operation)
{
}

template <typename T, typename R>
void recpp::processors::Reduce<T, R>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	auto forwardSubscription = recpp::subscriptions::ForwardSubscription(subscription);
	m_subscriber.onSubscribe(forwardSubscription);
}

template <typename T, typename R>
void recpp::processors::Reduce<T, R>::Impl::onNext(const T &value)
{
	m_current = m_operation(m_current, value);
}

template <typename T, typename R>
void recpp::processors::Reduce<T, R>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
}

template <typename T, typename R>
void recpp::processors::Reduce<T, R>::Impl::onComplete()
{
	m_subscriber.onNext(m_current);
	m_subscriber.onComplete();
}

template <typename T, typename R>
void recpp::processors::Reduce<T, R>::Impl::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
