#pragma once

#include <rscpp/Publisher.h>

#include <exception>

namespace recpp
{
	template <typename T>
	class ErrorPublisher : public rscpp::Publisher<T>
	{
	public:
		ErrorPublisher(const std::exception_ptr &error);

		void subscribe(rscpp::Subscriber<T> &subscriber) override;

	private:
		std::exception_ptr m_error;
	};
} // namespace recpp

#include <recpp/publishers/inl/ErrorPublisher.inl>
