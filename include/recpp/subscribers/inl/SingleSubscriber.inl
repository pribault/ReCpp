#pragma once

template <typename T>
recpp::SingleSubscriber<T>::SingleSubscriber(const rscpp::Subscriber<T> &subscriber)
	: m_subscriber(subscriber)
{
}

template <typename T>
void recpp::SingleSubscriber<T>::onNext(const T &value)
{
	if (!m_ended)
	{
		m_ended = true;
		m_subscriber.onNext(value);
		m_subscriber.onComplete();
	}
}

template <typename T>
void recpp::SingleSubscriber<T>::onError(const std::exception_ptr &error)
{
	if (!m_ended)
	{
		m_ended = true;
		m_subscriber.onError(error);
	}
}
