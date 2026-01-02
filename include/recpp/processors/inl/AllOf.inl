#pragma once

#include <recpp/subscriptions/ForwardSubscription.h>

template <typename T, typename R>
recpp::processors::AllOf<T, R>::AllOf(const rscpp::Publisher<T> &publisher, R predicate)
	: rscpp::Processor<T, bool>(std::make_shared<Impl>(*this, publisher, predicate))
{
}

template <typename T, typename R>
recpp::processors::AllOf<T, R>::Impl::Impl(rscpp::Processor<T, bool> &parent, const rscpp::Publisher<T> &publisher, R predicate)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_predicate(predicate)
{
}

template <typename T, typename R>
void recpp::processors::AllOf<T, R>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	m_subscription = recpp::subscriptions::ForwardSubscription(subscription);
	m_subscriber.onSubscribe(m_subscription);
}

template <typename T, typename R>
void recpp::processors::AllOf<T, R>::Impl::onNext(const T &value)
{
	if (!m_gotFalse && !m_predicate(value))
	{
		m_gotFalse = true;
		m_subscription.cancel();
	}
}

template <typename T, typename R>
void recpp::processors::AllOf<T, R>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
}

template <typename T, typename R>
void recpp::processors::AllOf<T, R>::Impl::onComplete()
{
	m_subscriber.onNext(!m_gotFalse);
	m_subscriber.onComplete();
	m_subscriber = {};
}

template <typename T, typename R>
void recpp::processors::AllOf<T, R>::Impl::subscribe(rscpp::Subscriber<bool> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
