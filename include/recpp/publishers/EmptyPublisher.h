#pragma once

#include <rscpp/Publisher.h>

 namespace recpp
{
	template <typename T>
	class EmptyPublisher : public rscpp::Publisher<T>
	{
	public:
		EmptyPublisher();

		void subscribe(rscpp::Subscriber<T> &subscriber) override;
	};
} // namespace recpp

#include <recpp/publishers/inl/EmptyPublisher.inl>
