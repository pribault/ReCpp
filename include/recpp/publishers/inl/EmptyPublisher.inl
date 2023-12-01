#pragma once

#include <recpp/subscriptions/EmptySubscription.h>

template <typename T>
recpp::publishers::EmptyPublisher<T>::EmptyPublisher() = default;

template <typename T>
void recpp::publishers::EmptyPublisher<T>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	auto subscription = recpp::subscriptions::EmptySubscription<T>(subscriber);
	subscriber.onSubscribe(subscription);
}
