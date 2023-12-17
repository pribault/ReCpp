#pragma once

#include <rscpp/Subscriber.h>
#include <rscpp/Subscription.h>

namespace recpp::subscriptions
{
	/**
	 * @class NeverSubscription NeverSubscription.h <recpp/subscriptions/NeverSubscription.h>
	 * @brief {@link rscpp::Subscription} that will never emit items nor complete. Completion will only happen on cancel.
	 * 
	 * @tparam T The type of element signaled to the {@link rscpp::Subscriber}.
	 */
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
		/**
		 * @brief Construct a new {@link NeverSubscription} instance.
		 * 
		 * @param subscriber The {@link rscpp::Subscriber} that will consume signals from this {@link rscpp::Subscription}.
		 */
		NeverSubscription(const rscpp::Subscriber<T> &subscriber);
	};
} // namespace recpp::subscriptions

#include <recpp/subscriptions/inl/NeverSubscription.inl>
