#pragma once

#include <limits>

template <typename T, typename R>
recpp::AndThen<T, R>::AndThen(const rscpp::Publisher<T> &source, const rscpp::Publisher<R> &dest)
	: rscpp::Processor<T, R>(std::shared_ptr<rscpp::Processor<T, R>>(new Impl(*this, source, dest)))
{
}

template <typename T, typename R>
recpp::AndThen<T, R>::Impl::Impl(rscpp::Processor<T, R> &parent, const rscpp::Publisher<T> &source, const rscpp::Publisher<R> &dest)
	: m_parent(parent)
	, m_source(source)
	, m_dest(dest)
{
}

template <typename T, typename R>
void recpp::AndThen<T, R>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	m_subscription = subscription;
	m_remaining = std::numeric_limits<size_t>::max();
	m_subscription.request(m_remaining);
}

template <typename T, typename R>
void recpp::AndThen<T, R>::Impl::onNext(const T &)
{
	m_remaining--;
	if (!m_remaining)
	{
		m_remaining = std::numeric_limits<size_t>::max();
		m_subscription.request(m_remaining);
	}
}

template <typename T, typename R>
void recpp::AndThen<T, R>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
}

template <typename T, typename R>
void recpp::AndThen<T, R>::Impl::onComplete()
{
	m_dest.subscribe(m_subscriber);
}

template <typename T, typename R>
void recpp::AndThen<T, R>::Impl::subscribe(rscpp::Subscriber<R> &subscriber)
{
	m_subscriber = subscriber;
	m_source.subscribe(m_parent);
}
