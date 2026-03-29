#pragma once

#include <recpp/subscriptions/ForwardSubscription.h>

template <typename T>
recpp::processors::Count<T>::Count(const rscpp::Publisher<T> &publisher)
	: rscpp::Processor<T, std::size_t>(std::make_shared<Impl>(*this, publisher))
{
}

template <typename T>
recpp::processors::Count<T>::Impl::Impl(rscpp::Processor<T, std::size_t> &parent, const rscpp::Publisher<T> &publisher)
	: m_parent(parent)
	, m_publisher(publisher)
{
}

template <typename T>
void recpp::processors::Count<T>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	m_subscription = recpp::subscriptions::ForwardSubscription(subscription);
	m_subscriber.onSubscribe(m_subscription);
}

template <typename T>
void recpp::processors::Count<T>::Impl::onNext(const T & /* value */)
{
	m_count++;
}

template <typename T>
void recpp::processors::Count<T>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
	m_subscription = {};
}

template <typename T>
void recpp::processors::Count<T>::Impl::onComplete()
{
	m_subscriber.onNext(m_count);
	m_subscriber.onComplete();
	m_subscription = {};
}

template <typename T>
void recpp::processors::Count<T>::Impl::subscribe(rscpp::Subscriber<std::size_t> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
