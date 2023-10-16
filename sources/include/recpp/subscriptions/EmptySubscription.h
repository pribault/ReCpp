#pragma once

#include <recpp/Subscription.h>

namespace recpp
{
	template <typename T>
	class EmptySubscription : public recpp::Subscription<T>
	{
	public:
		EmptySubscription(const rscpp::Subscriber<T> &subscriber);

	private:
		void internalRequest(size_t count) noexcept;
		void internalCancel() noexcept;
	};
} // namespace recpp

#include <recpp/subscriptions/EmptySubscription.inl>
