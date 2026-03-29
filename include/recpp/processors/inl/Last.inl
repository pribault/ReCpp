#pragma once

#include <recpp/subscriptions/ForwardSubscription.h>

template <typename T>
recpp::processors::Last<T>::Last(const rscpp::Publisher<T> &publisher)
	: rscpp::Processor<T, T>(std::make_shared<Impl>(*this, publisher))
{
}

template <typename T>
recpp::processors::Last<T>::Impl::Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher)
	: m_parent(parent)
	, m_publisher(publisher)
{
}

template <typename T>
void recpp::processors::Last<T>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	m_subscription = recpp::subscriptions::ForwardSubscription(subscription);
	m_subscriber.onSubscribe(m_subscription);
}

template <typename T>
void recpp::processors::Last<T>::Impl::onNext(const T &value)
{
	m_result.store(value);
}

template <typename T>
void recpp::processors::Last<T>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
	m_subscription = {};
}

template <typename T>
void recpp::processors::Last<T>::Impl::onComplete()
{
	const auto result = m_result.load();
	if (result)
		m_subscriber.onNext(*result);
	m_subscriber.onComplete();
	m_subscription = {};
}

template <typename T>
void recpp::processors::Last<T>::Impl::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
