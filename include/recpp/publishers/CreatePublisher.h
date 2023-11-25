#pragma once

#include <rscpp/Publisher.h>

#include <functional>

namespace recpp
{
	template <typename T, typename S>
	class CreatePublisher : public rscpp::Publisher<T>
	{
	public:
		using CreateMethod = std::function<void(S & /* subscriber */)>;

		CreatePublisher(const CreateMethod &method);

		void subscribe(rscpp::Subscriber<T> &subscriber) override;

	private:
		CreateMethod m_method;
	};
} // namespace recpp

#include <recpp/publishers/inl/CreatePublisher.inl>
