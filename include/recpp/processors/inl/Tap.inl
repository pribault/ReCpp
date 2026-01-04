#pragma once

#include <recpp/subscriptions/ForwardSubscription.h>

template <typename T>
recpp::processors::Tap<T>::Tap(const rscpp::Publisher<T> &publisher, const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod,
							   const OnCompleteMethod &onCompleteMethod)
	: rscpp::Processor<T, T>(std::make_shared<Impl>(*this, publisher, onNextMethod, onErrorMethod, onCompleteMethod))
{
}

template <typename T>
recpp::processors::Tap<T>::Impl::Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, const OnNextMethod &onNextMethod,
									  const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_onNextMethod(onNextMethod)
	, m_onErrorMethod(onErrorMethod)
	, m_onCompleteMethod(onCompleteMethod)
{
}

template <typename T>
void recpp::processors::Tap<T>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	auto forwardSubscription = recpp::subscriptions::ForwardSubscription(subscription);
	m_subscriber.onSubscribe(forwardSubscription);
}

template <typename T>
void recpp::processors::Tap<T>::Impl::onNext(const T &value)
{
	if (m_onNextMethod)
		m_onNextMethod(value);
	m_subscriber.onNext(value);
}

template <typename T>
void recpp::processors::Tap<T>::Impl::onError(const std::exception_ptr &error)
{
	if (m_onErrorMethod)
		m_onErrorMethod(error);
	m_subscriber.onError(error);
}

template <typename T>
void recpp::processors::Tap<T>::Impl::onComplete()
{
	if (m_onCompleteMethod)
		m_onCompleteMethod();
	m_subscriber.onComplete();
}

template <typename T>
void recpp::processors::Tap<T>::Impl::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
