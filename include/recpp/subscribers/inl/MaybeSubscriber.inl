#pragma once

template <typename T>
recpp::subscribers::MaybeSubscriber<T>::MaybeSubscriber(const rscpp::Subscriber<T> &subscriber)
	: m_subscriber(subscriber)
{
}

template <typename T>
void recpp::subscribers::MaybeSubscriber<T>::onNext(const T &value)
{
	if (!m_ended)
	{
		m_ended = true;
		m_subscriber.onNext(value);
		m_subscriber.onComplete();
	}
}

template <typename T>
void recpp::subscribers::MaybeSubscriber<T>::onError(const std::exception_ptr &error)
{
	if (!m_ended)
	{
		m_ended = true;
		m_subscriber.onError(error);
	}
}

template <typename T>
void recpp::subscribers::MaybeSubscriber<T>::onComplete()
{
	if (!m_ended)
	{
		m_ended = true;
		m_subscriber.onComplete();
	}
}
