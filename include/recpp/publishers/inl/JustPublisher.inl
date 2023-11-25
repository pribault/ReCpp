#pragma once

#include <recpp/subscriptions/JustSubscription.h>
#include <rscpp/Subscriber.h>

template <typename T>
recpp::JustPublisher<T>::JustPublisher(const T &value)
	: m_value(value)
{
}

template <typename T>
void recpp::JustPublisher<T>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	auto subscription = JustSubscription<T>(subscriber, m_value);
	subscriber.onSubscribe(subscription);
}
