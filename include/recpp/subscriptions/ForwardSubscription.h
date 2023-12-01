#pragma once

#include <rscpp/Subscription.h>

namespace recpp::subscriptions
{
	class ForwardSubscription : public rscpp::Subscription
	{
		class Impl : public rscpp::Subscription
		{
		public:
			Impl(const rscpp::Subscription &subscription);

			void request(size_t count) override;

			void cancel() override;

		private:
			rscpp::Subscription m_subscription;
		};

	public:
		ForwardSubscription(const rscpp::Subscription &subscription);
	};
} // namespace recpp::subscriptions
