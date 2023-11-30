#pragma once

#include <rscpp/Publisher.h>

namespace recpp::publishers
{
	template <typename T>
	class NeverPublisher : public rscpp::Publisher<T>
	{
	public:
		NeverPublisher();

		void subscribe(rscpp::Subscriber<T> &subscriber) override;
	};
} // namespace recpp::publishers

#include <recpp/publishers/inl/NeverPublisher.inl>
