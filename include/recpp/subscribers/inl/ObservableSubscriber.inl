#pragma once

template <typename T>
recpp::ObservableSubscriber<T>::ObservableSubscriber(const rscpp::Subscriber<T> &subscriber)
	: m_subscriber(subscriber)
{
}

template <typename T>
void recpp::ObservableSubscriber<T>::onNext(const T &value)
{
	if (!m_ended)
		m_subscriber.onNext(value);
}

template <typename T>
void recpp::ObservableSubscriber<T>::onError(const std::exception_ptr &error)
{
	if (!m_ended)
	{
		m_ended = true;
		m_subscriber.onError(error);
	}
}

template <typename T>
void recpp::ObservableSubscriber<T>::onComplete()
{
	if (!m_ended)
	{
		m_ended = true;
		m_subscriber.onComplete();
	}
}
