#pragma once

template <typename T, typename P>
recpp::publishers::DeferPublisher<T, P>::DeferPublisher(const std::function<P()> &function)
	: m_function(function)
{
}

template <typename T, typename P>
void recpp::publishers::DeferPublisher<T, P>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_function().rscpp::template Publisher<T>::subscribe(subscriber);
}
