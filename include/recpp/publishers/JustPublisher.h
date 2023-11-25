#pragma once

#include <rscpp/Publisher.h>

namespace recpp
{
	template <typename T>
	class JustPublisher : public rscpp::Publisher<T>
	{
	public:
		JustPublisher(const T &value);

		void subscribe(rscpp::Subscriber<T> &subscriber) override;

	private:
		T m_value;
	};
} // namespace recpp

#include <recpp/publishers/inl/JustPublisher.inl>
