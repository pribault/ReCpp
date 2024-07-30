#pragma once

template <typename T>
recpp::subscriptions::EmptySubscription<T>::EmptySubscription(const rscpp::Subscriber<T> &subscriber)
	: rscpp::Subscription(std::make_shared<Impl>(subscriber))
{
}

template <typename T>
recpp::subscriptions::EmptySubscription<T>::Impl::Impl(const rscpp::Subscriber<T> &subscriber)
	: m_subscriber(subscriber)
{
}

template <typename T>
void recpp::subscriptions::EmptySubscription<T>::Impl::request(size_t count)
{
	if (!m_completed)
	{
		m_completed = true;
		m_subscriber.onComplete();
	}
}

template <typename T>
void recpp::subscriptions::EmptySubscription<T>::Impl::cancel()
{
	if (!m_completed)
	{
		m_completed = true;
		m_subscriber.onComplete();
	}
}
