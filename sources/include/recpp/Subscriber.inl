#pragma once

template <typename T>
recpp::Subscriber<T>::Subscriber(const recpp::Subscriber<T>::OnNextMethod &onNextMethod, const recpp::Subscriber<T>::OnErrorMethod &onErrorMethod,
								 const recpp::Subscriber<T>::OnCompleteMethod &onCompleteMethod)
	: m_onNextMethod(onNextMethod)
	, m_onErrorMethod(onErrorMethod)
	, m_onCompleteMethod(onCompleteMethod)
{
}

template <typename T>
void recpp::Subscriber<T>::onSubscribe(rscpp::Subscription &subscription) noexcept
{
	subscription.request(1);
}

template <typename T>
void recpp::Subscriber<T>::onNext(const T &value) noexcept
{
	m_onNextMethod(value);
}

template <typename T>
void recpp::Subscriber<T>::onError(const std::exception_ptr &exception) noexcept
{
	m_onErrorMethod(exception);
}

template <typename T>
void recpp::Subscriber<T>::onComplete() noexcept
{
	m_onCompleteMethod();
}
