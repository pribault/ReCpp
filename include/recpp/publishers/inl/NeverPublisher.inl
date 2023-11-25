#pragma once

#include <recpp/subscriptions/NeverSubscription.h>
#include <rscpp/Subscriber.h>

template <typename T>
recpp::NeverPublisher<T>::NeverPublisher() = default;

template <typename T>
void recpp::NeverPublisher<T>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	auto subscription = NeverSubscription<T>(subscriber);
	subscriber.onSubscribe(subscription);
}
