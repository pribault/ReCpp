#pragma once

#include <rscpp/Subscriber.h>
#include <rscpp/Subscription.h>

namespace recpp::subscriptions
{
	/**
	 * @class EmptySubscription EmptySubscription.h <recpp/subscriptions/EmptySubscription.h>
	 * @brief {@link Subscription} that will emit no items but complete directly when items are requested.
	 * 
	 * @tparam T The type of element signaled to the {@link Subscriber}.
	 */
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
		/**
		 * @brief Construct a new {@link EmptySubscription} instance.
		 * 
		 * @param subscriber The {@link Subscriber} that will consume signals from this {@link Subscription}.
		 */
		EmptySubscription(const rscpp::Subscriber<T> &subscriber);
	};
} // namespace recpp::subscriptions

#include <recpp/subscriptions/inl/EmptySubscription.inl>
