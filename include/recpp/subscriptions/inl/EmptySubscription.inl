#pragma once

template <typename T>
recpp::EmptySubscription<T>::EmptySubscription(const rscpp::Subscriber<T> &subscriber)
	: rscpp::Subscription(std::shared_ptr<rscpp::Subscription>(new recpp::EmptySubscription<T>::Impl(subscriber)))
{
}

template <typename T>
recpp::EmptySubscription<T>::Impl::Impl(const rscpp::Subscriber<T> &subscriber)
	: m_subscriber(subscriber)
{
}

template <typename T>
void recpp::EmptySubscription<T>::Impl::request(size_t count)
{
	m_subscriber.onComplete();
}

template <typename T>
void recpp::EmptySubscription<T>::Impl::cancel()
{
	m_subscriber.onComplete();
}
