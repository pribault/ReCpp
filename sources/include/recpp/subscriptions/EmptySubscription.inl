#pragma once

template <typename T>
recpp::EmptySubscription<T>::EmptySubscription(const rscpp::Subscriber<T> &subscriber)
	: Subscription<T>(subscriber, std::bind(&EmptySubscription<T>::internalRequest, this, std::placeholders::_1), nullptr)
{
}

template <typename T>
void recpp::EmptySubscription<T>::internalRequest(size_t count) noexcept
{
	Subscription<T>::m_subscriber.onComplete();
}
