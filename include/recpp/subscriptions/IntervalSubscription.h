#pragma once

#include <rscpp/Publisher.h>
#include <rscpp/Subscriber.h>
#include <rscpp/Subscription.h>

#include <recpp/async/Scheduler.h>

#include <functional>
#include <optional>

namespace recpp::async
{
	class Scheduler;
}

namespace recpp::subscribers
{
	template <typename T>
	class DefaultSubscriber;
}

namespace recpp::subscriptions
{
	/**
	 * @class IntervalSubscription IntervalSubscription.h <recpp/subscriptions/IntervalSubscription.h>
	 * @brief {@link rscpp::Subscription} that will emit values at a specified interval.
	 *
	 * @tparam T The type of element signaled to the {@link rscpp::Subscriber}.
	 */
	template <typename T>
	class IntervalSubscription : public rscpp::Subscription
	{
		class Impl : public rscpp::Subscription
		{
		public:
			Impl(const rscpp::Subscriber<T> &subscriber, const recpp::async::Scheduler::TimePoint &start, const recpp::async::Scheduler::Duration &period,
				 recpp::async::Scheduler &scheduler);

			void request(size_t count) override;
			void cancel() override;

		private:
			void tryRequest(const std::optional<recpp::async::Scheduler::TimePoint> &prevPoint, T value);

			rscpp::Subscriber<T>			   m_subscriber;
			recpp::async::Scheduler::TimePoint m_start;
			recpp::async::Scheduler::Duration  m_period;
			recpp::async::Scheduler			  &m_scheduler;
			bool							   m_running = false;
			bool							   m_canceled = false;
			size_t							   m_requested = 0;
		};

	public:
		/**
		 * @brief Construct a new {@link IntervalSubscription} instance.
		 *
		 * @param subscriber The {@link rscpp::Subscriber} that will consume signals from this {@link rscpp::Subscription}.
		 * @param start The time point at which to start emitting values.
		 * @param period The period at which to emit the values.
		 * @param scheduler The scheduler to emit values on.
		 */
		IntervalSubscription(const rscpp::Subscriber<T> &subscriber, const recpp::async::Scheduler::TimePoint &start,
							 const recpp::async::Scheduler::Duration &period, recpp::async::Scheduler &scheduler);
	};
} // namespace recpp::subscriptions

#include <recpp/subscriptions/inl/IntervalSubscription.inl>
