#pragma once

#include <recpp/subscriptions/ForwardSubscription.h>

template <typename T, typename R>
recpp::processors::NoneOf<T, R>::NoneOf(const rscpp::Publisher<T> &publisher, R predicate)
	: rscpp::Processor<T, bool>(std::make_shared<Impl>(*this, publisher, predicate))
{
}

template <typename T, typename R>
recpp::processors::NoneOf<T, R>::Impl::Impl(rscpp::Processor<T, bool> &parent, const rscpp::Publisher<T> &publisher, R predicate)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_predicate(predicate)
{
}

template <typename T, typename R>
void recpp::processors::NoneOf<T, R>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	m_subscription = recpp::subscriptions::ForwardSubscription(subscription);
	m_subscriber.onSubscribe(m_subscription);
}

template <typename T, typename R>
void recpp::processors::NoneOf<T, R>::Impl::onNext(const T &value)
{
	if (!m_gotTrue && m_predicate(value))
	{
		m_gotTrue = true;
		m_subscription.cancel();
	}
}

template <typename T, typename R>
void recpp::processors::NoneOf<T, R>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
}

template <typename T, typename R>
void recpp::processors::NoneOf<T, R>::Impl::onComplete()
{
	m_subscriber.onNext(!m_gotTrue);
	m_subscriber.onComplete();
}

template <typename T, typename R>
void recpp::processors::NoneOf<T, R>::Impl::subscribe(rscpp::Subscriber<bool> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
