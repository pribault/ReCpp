#pragma once

template <typename T>
recpp::DefaultSubscriber<T>::DefaultSubscriber(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod)
	: rscpp::Subscriber<T>(std::shared_ptr<rscpp::Subscriber<T>>(new Impl(onNextMethod, onErrorMethod, onCompleteMethod)))
{
}

template <typename T>
recpp::DefaultSubscriber<T>::Impl::Impl(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod)
	: m_onNextMethod(onNextMethod)
	, m_onErrorMethod(onErrorMethod)
	, m_onCompleteMethod(onCompleteMethod)
{
}

template <typename T>
void recpp::DefaultSubscriber<T>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	m_subscription = subscription;
	m_remaining = std::numeric_limits<size_t>::max();
	m_subscription.request(m_remaining);
}

template <typename T>
void recpp::DefaultSubscriber<T>::Impl::onNext(const T &value)
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
void recpp::DefaultSubscriber<T>::Impl::onError(const std::exception_ptr &error)
{
	if (m_onErrorMethod)
		m_onErrorMethod(error);
}

template <typename T>
void recpp::DefaultSubscriber<T>::Impl::onComplete()
{
	if (m_onCompleteMethod)
		m_onCompleteMethod();
}
