#pragma once

template <typename T, typename S>
recpp::CreatePublisher<T, S>::CreatePublisher(const CreateMethod &method)
	: m_method(method)
{
}

template <typename T, typename S>
void recpp::CreatePublisher<T, S>::subscribe(rscpp::Subscriber<T> &subscriber)
{
	auto s = S(subscriber);
	m_method(s);
}
