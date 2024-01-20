#pragma once

#include <recpp/async/Schedulable.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <map>
#include <mutex>
#include <optional>

namespace recpp::async
{
	/**
	 * @class SchedulableQueue SchedulableQueue.h <recpp/async/SchedulableQueue.h>
	 * @brief Represents a {@link Schedulable} priority queue.
	 * <p>
	 * This class is made to be used with the a {@link Scheduler}.
	 */
	class SchedulableQueue
	{
	public:
		/**
		 * @brief The clock used by the {@link SchedulableQueue}.
		 */
		using Clock = std::chrono::steady_clock;

		/**
		 * @brief The clock duration used by the {@link SchedulableQueue}.
		 */
		using Duration = Clock::duration;

		/**
		 * @brief The clock time point used by the {@link SchedulableQueue}.
		 */
		using TimePoint = Clock::time_point;

		/**
		 * @brief Push a {@link Schedulable} to execute after the specified time point.
		 *
		 * @param timePoint The time point to execute the {@link Schedulable} after.
		 * @param schedulable The {@link Schedulable} to execute.
		 */
		void push(const TimePoint &timePoint, const Schedulable &schedulable);

		/**
		 * @brief Try to pop a {@link Schedulable} from the queue, waiting for the specified maximum duration.
		 *
		 * @param duration The maximum duration to wait a {@link Schedulable} for.
		 * @return std::optional<Schedulable> An optional {@link Schedulable}.
		 */
		std::optional<Schedulable> pop(const Duration &duration);

		/**
		 * @brief Try to pop a {@link Schedulable} from the queue, waiting until the specified maximum time point.
		 *
		 * @param until The maximum time point to wait a {@link Schedulable} for.
		 * @return std::optional<Schedulable> An optional {@link Schedulable}.
		 */
		std::optional<Schedulable> pop(const TimePoint &until);

		/**
		 * @brief Try to pop a {@link Schedulable} from the queue, waiting indefinitely for it.
		 * <p>
		 * This method is blocking until the {@link #stop} method is called.
		 *
		 * @return std::optional<Schedulable> An optional {@link Schedulable}.
		 */
		std::optional<Schedulable> blockingPop();

		/**
		 * @brief Get the queue size.
		 *
		 * @return size_t The queue size
		 */
		size_t size();

		/**
		 * @brief Stop this queue and wake up all threads trying to pop an element.
		 * <p>
		 * After this call threads will not be able to wait for elements anymore.
		 */
		void stop();

		/**
		 * @brief Wake up all threads trying to pop an element.
		 */
		void notifyAll();

	private:
		/**
		 * @brief Internal method to try to pop a {@link Schedulable} from the queue.
		 *
		 * @param timepoint The timepoint before which we are trying to pop a {@link Schedulable}.
		 * @return std::optional<Schedulable> An optional {@link Schedulable}.
		 */
		std::optional<Schedulable> tryPop(const TimePoint &timePoint);

		/**
		 * @brief The {@link Schedulable} objects queue.
		 */
		std::multimap<TimePoint, Schedulable> m_schedulables;

		/**
		 * @brief The mutex to protect access to shared data.
		 */
		std::mutex m_dataMutex;

		/**
		 * @brief The mutex used with the condition variable to wait for a {@link Schedulable} to be pushed or a timeout to expire.
		 */
		std::mutex m_cvMutex;

		/**
		 * @brief The condition variable to wait for a {@link Schedulable} to be pushed or a timeout to expire.
		 */
		std::condition_variable m_cv;

		/**
		 * @brief Is this queue stopped ?
		 */
		std::atomic_bool m_stop = false;
	};
} // namespace recpp::async
