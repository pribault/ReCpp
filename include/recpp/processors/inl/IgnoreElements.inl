#pragma once

#include <limits>

template <typename T, typename R>
recpp::processors::IgnoreElements<T, R>::IgnoreElements(const rscpp::Publisher<T> &source)
	: rscpp::Processor<T, R>(std::make_shared<Impl>(*this, source))
{
}

template <typename T, typename R>
recpp::processors::IgnoreElements<T, R>::Impl::Impl(rscpp::Processor<T, R> &parent, const rscpp::Publisher<T> &source)
	: m_parent(parent)
	, m_source(source)
{
}

template <typename T, typename R>
void recpp::processors::IgnoreElements<T, R>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	m_subscription = subscription;
	m_remaining = std::numeric_limits<size_t>::max();
	m_subscription.request(m_remaining);
}

template <typename T, typename R>
void recpp::processors::IgnoreElements<T, R>::Impl::onNext(const T &)
{
	m_remaining--;
	if (!m_remaining)
	{
		m_remaining = std::numeric_limits<size_t>::max();
		m_subscription.request(m_remaining);
	}
}

template <typename T, typename R>
void recpp::processors::IgnoreElements<T, R>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
}

template <typename T, typename R>
void recpp::processors::IgnoreElements<T, R>::Impl::onComplete()
{
	m_subscriber.onComplete();
}

template <typename T, typename R>
void recpp::processors::IgnoreElements<T, R>::Impl::subscribe(rscpp::Subscriber<R> &subscriber)
{
	m_subscriber = subscriber;
	m_source.subscribe(m_parent);
}
