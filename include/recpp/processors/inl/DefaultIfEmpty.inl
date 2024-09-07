#pragma once

#include <recpp/subscriptions/ForwardSubscription.h>

template <typename T>
recpp::processors::DefaultIfEmpty<T>::DefaultIfEmpty(const rscpp::Publisher<T> &publisher, const T &defaultValue)
	: rscpp::Processor<T, T>(std::make_shared<Impl>(*this, publisher, defaultValue))
{
}

template <typename T>
recpp::processors::DefaultIfEmpty<T>::Impl::Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, const T &defaultValue)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_defaultValue(defaultValue)
{
}

template <typename T>
void recpp::processors::DefaultIfEmpty<T>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	auto forwardSubscription = recpp::subscriptions::ForwardSubscription(subscription);
	m_subscriber.onSubscribe(forwardSubscription);
}

template <typename T>
void recpp::processors::DefaultIfEmpty<T>::Impl::onNext(const T &value)
{
	m_gotValues = true;
	m_subscriber.onNext(value);
}

template <typename T>
void recpp::processors::DefaultIfEmpty<T>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
}

template <typename T>
void recpp::processors::DefaultIfEmpty<T>::Impl::onComplete()
{
	if (!m_gotValues)
		m_subscriber.onNext(m_defaultValue);
	m_subscriber.onComplete();
}

template <typename T>
void recpp::processors::DefaultIfEmpty<T>::Impl::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
