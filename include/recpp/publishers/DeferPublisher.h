#pragma once

#include <rscpp/Publisher.h>

#include <functional>

namespace recpp
{
	template <typename T, typename P>
	class DeferPublisher : public rscpp::Publisher<T>
	{
	public:
		DeferPublisher(const std::function<P()> &function);

		void subscribe(rscpp::Subscriber<T> &subscriber) override;

	private:
		std::function<P()> m_function;
	};
} // namespace recpp

#include <recpp/publishers/inl/DeferPublisher.inl>
