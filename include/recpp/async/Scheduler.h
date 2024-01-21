#pragma once

#include <recpp/async/SchedulableQueue.h>

namespace recpp::async
{
	class Schedulable;

	/**
	 * @class Scheduler Scheduler.h <recpp/async/Scheduler.h>
	 * @brief Represents an object that can manage and execute {@link Schedulable} objects.
	 * <p>
	 * This class is made to be inherited from.
	 */
	class Scheduler
	{
	public:
		/**
		 * @brief The clock used by the {@link Scheduler}.
		 */
		using Clock = SchedulableQueue::Clock;

		/**
		 * @brief The clock duration used by the {@link Scheduler}.
		 */
		using Duration = SchedulableQueue::Duration;

		/**
		 * @brief The clock time point used by the {@link Scheduler}.
		 */
		using TimePoint = SchedulableQueue::TimePoint;

		/**
		 * @brief Schedule a {@link Schedulable} execution with no time specification.
		 *
		 * @param schedulable The {@link Schedulable} to execute.
		 */
		void schedule(const Schedulable &schedulable);

		/**
		 * @brief Schedule a {@link Schedulable} to be executed after the specified time point.
		 *
		 * @param timepoint The time point to execute the {@link Schedulable} after.
		 * @param schedulable The {@link Schedulable} to execute.
		 */
		void schedule(const TimePoint &timepoint, const Schedulable &schedulable);

		/**
		 * @brief Schedule a {@link Schedulable} to be executed after the specified duration starting from now.
		 *
		 * @param duration The duration to execute the {@link Schedulable} after.
		 * @param schedulable The {@link Schedulable} to execute.
		 */
		void schedule(const Duration &duration, const Schedulable &schedulable);

	protected:
		/**
		 * @brief The {@link SchedulableQueue} to manage the {@link Schedulable} objects.
		 */
		SchedulableQueue m_queue;
	};
} // namespace recpp::async
