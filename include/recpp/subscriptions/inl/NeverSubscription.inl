#pragma once

template <typename T>
recpp::subscriptions::NeverSubscription<T>::NeverSubscription(const rscpp::Subscriber<T> &subscriber)
	: rscpp::Subscription(std::make_shared<Impl>(subscriber))
{
}

template <typename T>
recpp::subscriptions::NeverSubscription<T>::Impl::Impl(const rscpp::Subscriber<T> &subscriber)
	: m_subscriber(subscriber)
{
}

template <typename T>
void recpp::subscriptions::NeverSubscription<T>::Impl::request(size_t count)
{
}

template <typename T>
void recpp::subscriptions::NeverSubscription<T>::Impl::cancel()
{
	m_subscriber.onComplete();
}
