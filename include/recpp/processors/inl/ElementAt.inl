#pragma once

#include <recpp/subscriptions/ForwardSubscription.h>

template <typename T>
recpp::processors::ElementAt<T>::ElementAt(const rscpp::Publisher<T> &publisher, std::size_t index)
	: rscpp::Processor<T, T>(std::make_shared<Impl>(*this, publisher, index))
{
}

template <typename T>
recpp::processors::ElementAt<T>::Impl::Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, std::size_t index)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_distance(index)
{
}

template <typename T>
void recpp::processors::ElementAt<T>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	m_subscription = recpp::subscriptions::ForwardSubscription(subscription);
	m_subscriber.onSubscribe(m_subscription);
}

template <typename T>
void recpp::processors::ElementAt<T>::Impl::onNext(const T &value)
{
	if (m_distance-- == 0)
	{
		m_result = value;
		m_subscription.cancel();
	}
}

template <typename T>
void recpp::processors::ElementAt<T>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
}

template <typename T>
void recpp::processors::ElementAt<T>::Impl::onComplete()
{
	m_subscriber.onNext(m_result ? *m_result : T{});
	m_subscriber.onComplete();
}

template <typename T>
void recpp::processors::ElementAt<T>::Impl::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
