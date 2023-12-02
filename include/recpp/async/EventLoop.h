#pragma once

#include <recpp/async/Scheduler.h>

#include <atomic>

namespace recpp::async
{
	/**
	 * @class EventLoop EventLoop.h <recpp/async/EventLoop.h>
	 * @brief Represents a "manual" {@link Scheduler} you can use with your main thread or a self managed thread.
	 * <p>
	 * All {@link Schedulable} objects will be run on the thread running the {@link EventLoop}.
	 * <p>
	 * The typical use of an {@link EventLoop} is on a main thread to be able to execute {@link Schedulable} objects on it.
	 */
	class EventLoop : public Scheduler
	{
	public:
		using Clock = Scheduler::Clock;
		using Duration = Scheduler::Duration;
		using TimePoint = Scheduler::TimePoint;

		/**
		 * @brief Run indefinitely this {@link EventLoop}
		 */
		void run();

		/**
		 * @brief Run this {@link EventLoop} for at least the specified duration.
		 *
		 * @param duration The duration to run this {@link EventLoop} for.
		 */
		void runFor(const Duration &duration);

		/**
		 * @brief Stop this {@link EventLoop}.
		 * <p>
		 * This {@link EventLoop} will not be able to run any {@link Schedulable} after this call.
		 */
		void stop();

	private:
		/**
		 * @brief Is this {@link EventLoop} stopped ?
		 */
		std::atomic_bool m_stop = false;
	};
} // namespace recpp::async
