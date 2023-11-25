#pragma once

#include <rscpp/Subscription.h>

namespace recpp
{
	class FilterSubscription : public rscpp::Subscription
	{
		class Impl : public rscpp::Subscription
		{
		public:
			Impl(const rscpp::Subscription &subscription);

			void request(size_t count) override;

			void cancel() override;

			void onNext(bool filtered);

		private:
			rscpp::Subscription m_subscription;
			size_t				m_requested = 0;
			size_t				m_waiting = 0;
		};

	public:
		FilterSubscription(const rscpp::Subscription &subscription);

		void onNext(bool filtered);
	};
} // namespace recpp
