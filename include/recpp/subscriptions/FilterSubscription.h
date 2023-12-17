#pragma once

#include <rscpp/Subscription.h>

namespace recpp::subscriptions
{
	/**
	 * @class FilterSubscription FilterSubscription.h <recpp/subscriptions/FilterSubscription.h>
	 * @brief {@link Subscription} that will be used by a {@link Filter} to filter the emitted items.
	 */
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
		/**
		 * @brief Construct a new {@link FilterSubscription} instance.
		 *
		 * @param subscription The source {@link Subscription} used to request items when necessary.
		 */
		FilterSubscription(const rscpp::Subscription &subscription);

		/**
		 * @brief Method called by the {@link Filter} to notify that an item was emitted.
		 *
		 * @param filtered Boolean defining if the item is filtered.
		 */
		void onNext(bool filtered);
	};
} // namespace recpp::subscriptions
