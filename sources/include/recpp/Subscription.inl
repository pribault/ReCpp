#pragma once

template <typename T>
recpp::Subscription<T>::Subscription(const typename rscpp::Subscriber<T> &subscriber, const typename rscpp::Subscription::RequestMethod &requestMethod,
									 const typename rscpp::Subscription::CancelMethod &cancelMethod)
	: rscpp::Subscription(requestMethod, cancelMethod)
	, m_subscriber(subscriber)
{
}
