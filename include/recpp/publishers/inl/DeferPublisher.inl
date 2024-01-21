#pragma once

template <typename T, typename P>
recpp::publishers::DeferPublisher<T, P>::DeferPublisher(const std::function<P()> &method)
	: m_method(method)
{
}

template <typename T, typename P>
void recpp::publishers::DeferPublisher<T, P>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_method().rscpp::template Publisher<T>::subscribe(subscriber);
}
