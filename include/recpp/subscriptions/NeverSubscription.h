#pragma once

#include <rscpp/Subscriber.h>
#include <rscpp/Subscription.h>

namespace recpp::subscriptions
{
	template <typename T>
	class NeverSubscription : public rscpp::Subscription
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
		NeverSubscription(const rscpp::Subscriber<T> &subscriber);
	};
} // namespace recpp::subscriptions

#include <recpp/subscriptions/inl/NeverSubscription.inl>
