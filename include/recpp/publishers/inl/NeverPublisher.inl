#pragma once

#include <recpp/subscriptions/NeverSubscription.h>
#include <rscpp/Subscriber.h>

template <typename T>
recpp::publishers::NeverPublisher<T>::NeverPublisher() = default;

template <typename T>
void recpp::publishers::NeverPublisher<T>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	auto subscription = recpp::subscriptions::NeverSubscription<T>(subscriber);
	subscriber.onSubscribe(subscription);
}
