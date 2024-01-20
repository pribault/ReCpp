#pragma once

#include <atomic>
#include <thread>

namespace recpp::async
{
	class SchedulableQueue;

	/**
	 * @class ThreadPoolWorker ThreadPoolWorker.h <recpp/async/ThreadPoolWorker.h>
	 * @brief Represents a worker thread used by a {@link ThreadPool}.
	 */
	class ThreadPoolWorker
	{
	public:
		/**
		 * @brief Construct a new {@link ThreadPoolWorker} instance that will use the given {@link SchedulableQueue}.
		 *
		 * @param queue The {@link SchedulableQueue} to use.
		 */
		explicit ThreadPoolWorker(SchedulableQueue &queue);

		/**
		 * @brief Destroy this {@link ThreadPoolWorker} instance.
		 */
		~ThreadPoolWorker();

		/**
		 * @brief Stop this {@link ThreadPoolWorker} instance.
		 */
		void stop();

	private:
		/**
		 * @brief Run this {@link ThreadPoolWorker} instance.
		 * <p>
		 * This is the method given to the std::thread.
		 */
		void run();

		/**
		 * @brief The {@link SchedulableQueue} used by this {@link Scheduler}.
		 */
		SchedulableQueue &m_queue;

		/**
		 * @brief The thread object.
		 */
		std::thread m_thread;

		/**
		 * @brief Is this {@link ThreadPoolWorker} instance stopped ?
		 */
		std::atomic<bool> m_stop;
	};
} // namespace recpp::async
