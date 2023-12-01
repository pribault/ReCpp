#pragma once

#include <rscpp/Publisher.h>

namespace recpp::publishers
{
	template <typename T>
	class EmptyPublisher : public rscpp::Publisher<T>
	{
	public:
		EmptyPublisher();

		void subscribe(rscpp::Subscriber<T> &subscriber) override;
	};
} // namespace recpp::publishers

#include <recpp/publishers/inl/EmptyPublisher.inl>
