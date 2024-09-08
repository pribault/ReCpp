#pragma once

#include <recpp/subscriptions/ForwardSubscription.h>

template <typename T, typename R>
recpp::processors::Min<T, R>::Min(const rscpp::Publisher<T> &publisher, R comparator)
	: rscpp::Processor<T, T>(std::make_shared<Impl>(*this, publisher, comparator))
{
}

template <typename T, typename R>
recpp::processors::Min<T, R>::Impl::Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, R comparator)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_comparator(comparator)
{
}

template <typename T, typename R>
void recpp::processors::Min<T, R>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	m_subscription = recpp::subscriptions::ForwardSubscription(subscription);
	m_subscriber.onSubscribe(m_subscription);
}

template <typename T, typename R>
void recpp::processors::Min<T, R>::Impl::onNext(const T &value)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (!m_min || m_comparator(value, *m_min))
		m_min = value;
}

template <typename T, typename R>
void recpp::processors::Min<T, R>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
}

template <typename T, typename R>
void recpp::processors::Min<T, R>::Impl::onComplete()
{
	m_subscriber.onNext(m_min ? *m_min : T{});
	m_subscriber.onComplete();
}

template <typename T, typename R>
void recpp::processors::Min<T, R>::Impl::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
