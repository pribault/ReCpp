#pragma once

#include <rscpp/Subscription.h>

namespace recpp::subscriptions
{
	/**
	 * @class ForwardSubscription ForwardSubscription.h <recpp/subscriptions/ForwardSubscription.h>
	 * @brief {@link rscpp::Subscription} that is used to forward requests to another {@link rscpp::Subscription}.
	 * <p>
	 * This class is mainly used for {@link rscpp::Processor}.
	 */
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
		/**
		 * @brief Construct a new {@link ForwardSubscription} instance.
		 *
		 * @param subscription The {@link rscpp::Subscription} to forward requests to.
		 */
		ForwardSubscription(const rscpp::Subscription &subscription);
	};
} // namespace recpp::subscriptions
