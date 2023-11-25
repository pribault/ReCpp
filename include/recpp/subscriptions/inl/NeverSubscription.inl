#pragma once

template <typename T>
recpp::NeverSubscription<T>::NeverSubscription(const rscpp::Subscriber<T> &subscriber)
	: rscpp::Subscription(std::shared_ptr<rscpp::Subscription>(new Impl(subscriber)))
{
}

template <typename T>
recpp::NeverSubscription<T>::Impl::Impl(const rscpp::Subscriber<T> &subscriber)
	: m_subscriber(subscriber)
{
}

template <typename T>
void recpp::NeverSubscription<T>::Impl::request(size_t count)
{
}

template <typename T>
void recpp::NeverSubscription<T>::Impl::cancel()
{
	m_subscriber.onComplete();
}
