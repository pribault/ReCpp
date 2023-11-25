#pragma once

#include <recpp/subscriptions/EmptySubscription.h>

template <typename T>
recpp::EmptyPublisher<T>::EmptyPublisher() = default;

template <typename T>
void recpp::EmptyPublisher<T>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	auto subscription = EmptySubscription<T>(subscriber);
	subscriber.onSubscribe(subscription);
}
