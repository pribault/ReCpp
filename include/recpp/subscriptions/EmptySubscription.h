#pragma once

#include <rscpp/Subscriber.h>
#include <rscpp/Subscription.h>

namespace recpp
{
	template <typename T>
	class EmptySubscription : public rscpp::Subscription
	{
		class Impl : public rscpp::Subscription
		{
		public:
			Impl(const rscpp::Subscriber<T> &subscriber);

			void request(size_t count) override;

			void cancel() override;

		private:
			rscpp::Subscriber<T> m_subscriber;
		};

	public:
		EmptySubscription(const rscpp::Subscriber<T> &subscriber);
	};
} // namespace recpp

#include <recpp/subscriptions/inl/EmptySubscription.inl>
