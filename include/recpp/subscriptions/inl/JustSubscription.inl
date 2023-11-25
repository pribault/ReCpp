#pragma once

template <typename T>
recpp::JustSubscription<T>::JustSubscription(const rscpp::Subscriber<T> &subscriber, const T &value)
	: rscpp::Subscription(std::shared_ptr<Subscription>(new Impl(subscriber, value)))
{
}

template <typename T>
recpp::JustSubscription<T>::Impl::Impl(const rscpp::Subscriber<T> &subscriber, const T &value)
	: m_subscriber(subscriber)
	, m_value(value)
{
}

template <typename T>
void recpp::JustSubscription<T>::Impl::request(size_t count)
{
	if (m_completed)
		return;
	m_completed = true;
	m_subscriber.onNext(m_value);
	m_subscriber.onComplete();
}

template <typename T>
void recpp::JustSubscription<T>::Impl::cancel()
{
	m_subscriber.onComplete();
}
