#pragma once

#include <rscpp/Publisher.h>

namespace recpp
{
	template <typename T, class I>
	class RangePublisher : public rscpp::Publisher<T>
	{
	public:
		RangePublisher(I first, I last);

		void subscribe(rscpp::Subscriber<T> &subscriber) override;

	private:
		I m_first;
		I m_last;
	};
} // namespace recpp

#include <recpp/publishers/inl/RangePublisher.inl>
