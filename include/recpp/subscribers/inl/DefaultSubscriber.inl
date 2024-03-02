#pragma once

#include <limits>

template <typename T>
recpp::subscribers::DefaultSubscriber<T>::DefaultSubscriber(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod,
															const OnCompleteMethod &onCompleteMethod, const OnSubscribeMethod &onSubscribeMethod)
	: rscpp::Subscriber<T>(std::make_shared<Impl>(onNextMethod, onErrorMethod, onCompleteMethod, onSubscribeMethod))
{
}

template <typename T>
recpp::subscribers::DefaultSubscriber<T>::Impl::Impl(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod,
													 const OnCompleteMethod &onCompleteMethod, const OnSubscribeMethod &onSubscribeMethod)
	: m_onNextMethod(onNextMethod)
	, m_onErrorMethod(onErrorMethod)
	, m_onCompleteMethod(onCompleteMethod)
	, m_onSubscribeMethod(onSubscribeMethod)
{
}

template <typename T>
void recpp::subscribers::DefaultSubscriber<T>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	m_subscription = subscription;
	if (m_onSubscribeMethod)
		m_onSubscribeMethod(subscription);
	m_remaining = std::numeric_limits<size_t>::max();
	m_subscription.request(m_remaining);
}

template <typename T>
void recpp::subscribers::DefaultSubscriber<T>::Impl::onNext(const T &value)
{
	m_remaining--;
	if (m_onNextMethod)
		m_onNextMethod(value);
	if (!m_remaining)
	{
		m_remaining = std::numeric_limits<size_t>::max();
		m_subscription.request(m_remaining);
	}
}

template <typename T>
void recpp::subscribers::DefaultSubscriber<T>::Impl::onError(const std::exception_ptr &error)
{
	if (m_onErrorMethod)
		m_onErrorMethod(error);
}

template <typename T>
void recpp::subscribers::DefaultSubscriber<T>::Impl::onComplete()
{
	if (m_onCompleteMethod)
		m_onCompleteMethod();
}
