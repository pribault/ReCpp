#pragma once

#include <rscpp/Publisher.h>

#include <recpp/async/Scheduler.h>

#include <functional>
#include <optional>

namespace recpp::async
{
	class Scheduler;
}

namespace recpp::publishers
{
	/**
	 * @class IntervalPublisher IntervalPublisher.h <recpp/publishers/IntervalPublisher.h>
	 * @brief {@link rscpp::Publisher} that will emit values at a specified interval.
	 *
	 * @tparam T The type of element signaled by the {@link rscpp::Publisher}.
	 */
	template <typename T>
	class IntervalPublisher : public rscpp::Publisher<T>
	{
	public:
		/**
		 * @brief Construct a new {@link IntervalPublisher} instance.
		 *
		 * @param start The time point at which to start emitting values.
		 * @param period The period at which to emit the values.
		 * @param scheduler The scheduler to emit values on.
		 */
		IntervalPublisher(const recpp::async::Scheduler::TimePoint &start, const recpp::async::Scheduler::Duration &period, recpp::async::Scheduler &scheduler);

		/**
		 * @brief Request {@link rscpp::Publisher} to start streaming data.
		 * <p>
		 * This is a "factory method" and can be called multiple times, each time starting a new {@link rscpp::Subscription}.
		 * <p>
		 * Each {@link rscpp::Subscription} will work for only a single {@link rscpp::Subscriber}.
		 * <p>
		 * A {@link rscpp::Subscriber} should only subscribe once to a single {@link rscpp::Publisher}.
		 * <p>
		 * If the {@link rscpp::Publisher} rejects the subscription attempt or otherwise fails it will
		 * signal the error via {@link rscpp::Subscriber::onError(const std::exception_ptr &)}.
		 *
		 * @param subscriber The {@link rscpp::Subscriber} that will consume signals from this {@link rscpp::Publisher}
		 */
		void subscribe(rscpp::Subscriber<T> &subscriber) override;

	private:
		recpp::async::Scheduler::TimePoint m_start;
		recpp::async::Scheduler::Duration  m_period;
		recpp::async::Scheduler			  &m_scheduler;
	};
} // namespace recpp::publishers

#include <recpp/publishers/inl/IntervalPublisher.inl>
