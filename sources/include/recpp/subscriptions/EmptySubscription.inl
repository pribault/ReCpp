#pragma once

template <typename T>
recpp::EmptySubscription<T>::EmptySubscription(const rscpp::Subscriber<T> &subscriber)
	: recpp::Subscription<T>(
		  subscriber, [this](size_t count) { internalRequest(count); }, [this]() { internalCancel(); })
{
}

template <typename T>
void recpp::EmptySubscription<T>::internalRequest(size_t count) noexcept
{
	recpp::Subscription<T>::m_subscriber.onComplete();
}

template <typename T>
void recpp::EmptySubscription<T>::internalCancel() noexcept
{
	recpp::Subscription<T>::m_subscriber.onComplete();
}
