#pragma once

#include <rscpp/Publisher.h>

namespace recpp
{
	template <typename T>
	class NeverPublisher : public rscpp::Publisher<T>
	{
	public:
		NeverPublisher();

		void subscribe(rscpp::Subscriber<T> &subscriber) override;
	};
} // namespace recpp

#include <recpp/publishers/inl/NeverPublisher.inl>
