#pragma once

template <typename T, typename P>
recpp::DeferPublisher<T, P>::DeferPublisher(const std::function<P()> &function)
	: m_function(function)
{
}

template <typename T, typename P>
void recpp::DeferPublisher<T, P>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_function().rscpp::Publisher<T>::subscribe(subscriber);
}
