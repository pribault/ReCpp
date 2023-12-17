#pragma once

#include <rscpp/Subscriber.h>
#include <rscpp/Subscription.h>

namespace recpp::subscriptions
{
	/**
	 * @class RangeSubscription RangeSubscription.h <recpp/subscriptions/RangeSubscription.h>
	 * @brief {@link Subscription} that will emit a range of items when requested.
	 *
	 * @tparam T The type of element signaled to the {@link Subscriber}.
	 * @tparam I The type of the range iterator.
	 */
	template <typename T, class I>
	class RangeSubscription : public rscpp::Subscription
	{
		class Impl : public rscpp::Subscription
		{
		public:
			Impl(const rscpp::Subscriber<T> &subscriber, I first, I last);

			void request(size_t count) override;

			void cancel() override;

		private:
			rscpp::Subscriber<T> m_subscriber;
			I					 m_current;
			I					 m_last;
			bool				 m_completed = false;
			bool				 m_canceled = false;
		};

	public:
		/**
		 * @brief Construct a new {@link RangeSubscription} instance.
		 *
		 * @param subscriber The {@link Subscriber} that will consume signals from this {@link Subscription}.
		 * @param first The first iterator.
		 * @param last The last iterator.
		 */
		RangeSubscription(const rscpp::Subscriber<T> &subscriber, I first, I last);
	};
} // namespace recpp::subscriptions

#include <recpp/subscriptions/inl/RangeSubscription.inl>
