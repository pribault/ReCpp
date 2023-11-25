#pragma once

#include <recpp/subscriptions/ForwardSubscription.h>

template <typename T, typename R>
recpp::Map<T, R>::Map(const rscpp::Publisher<T> &publisher, const std::function<R(const T & /* value */)> &method)
	: rscpp::Processor<T, R>(std::shared_ptr<rscpp::Processor<T, R>>(new Impl(*this, publisher, method)))
{
}

template <typename T, typename R>
recpp::Map<T, R>::Impl::Impl(rscpp::Processor<T, R> &parent, const rscpp::Publisher<T> &publisher, const std::function<R(const T & /* value */)> &method)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_method(method)
{
}

template <typename T, typename R>
void recpp::Map<T, R>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	auto forwardSubscription = recpp::ForwardSubscription(subscription);
	m_subscriber.onSubscribe(forwardSubscription);
}

template <typename T, typename R>
void recpp::Map<T, R>::Impl::onNext(const T &value)
{
	m_subscriber.onNext(m_method(value));
}

template <typename T, typename R>
void recpp::Map<T, R>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
}

template <typename T, typename R>
void recpp::Map<T, R>::Impl::onComplete()
{
	m_subscriber.onComplete();
}

template <typename T, typename R>
void recpp::Map<T, R>::Impl::subscribe(rscpp::Subscriber<R> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
