#pragma once

#include <rscpp/Subscriber.h>

template <typename T>
recpp::publishers::ErrorPublisher<T>::ErrorPublisher(const std::exception_ptr &error)
	: m_error(error)
{
}

template <typename T>
void recpp::publishers::ErrorPublisher<T>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	subscriber.onError(m_error);
}
