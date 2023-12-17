#pragma once

#include <rscpp/Subscriber.h>
#include <rscpp/Subscription.h>

namespace recpp::subscriptions
{
	/**
	 * @class JustSubscription JustSubscription.h <recpp/subscriptions/JustSubscription.h>
	 * @brief {@link Subscription} that will pass only one item to the {@link Subscriber} before completing.
	 * 
	 * @tparam T The type of element signaled to the {@link Subscriber}.
	 */
	template <typename T>
	class JustSubscription : public rscpp::Subscription
	{
		class Impl : public rscpp::Subscription
		{
		public:
			Impl(const rscpp::Subscriber<T> &subscriber, const T &value);

			void request(size_t count) override;

			void cancel() override;

		private:
			rscpp::Subscriber<T> m_subscriber;
			T					 m_value;
			bool				 m_completed = false;
		};

	public:
		/**
		 * @brief Construct a new {@link JustSubscription} instance.
		 * 
		 * @param subscriber The {@link Subscriber} that will consume signals from this {@link Subscription}.
		 * @param value The value to emit.
		 */
		JustSubscription(const rscpp::Subscriber<T> &subscriber, const T &value);
	};
} // namespace recpp::subscriptions

#include <recpp/subscriptions/inl/JustSubscription.inl>
