#pragma once

#include <recpp/async/Scheduler.h>

#include <atomic>
#include <thread>

namespace recpp::async
{
	/**
	 * @class WorkerThread WorkerThread.h <recpp/async/WorkerThread.h>
	 * @brief Represents an independant {@link Scheduler} running in a dedicated thread.
	 */
	class WorkerThread : public Scheduler
	{
	public:
		/**
		 * @brief {@link WorkerThread} default Constructor.
		 */
		WorkerThread();

		/**
		 * @brief Destroy this {@link WorkerThread} instance.
		 */
		~WorkerThread();

		/**
		 * @brief Stop this {@link WorkerThread} instance.
		 */
		void stop();

		/**
		 * @brief Get the thread id.
		 *
		 * @return std::thread::id The thread id.
		 */
		std::thread::id threadId() const;

	private:
		/**
		 * @brief Run this {@link WorkerThread} instance.
		 * <p>
		 * This is the method given to the std::thread.
		 */
		void run();

		/**
		 * @brief The thread object.
		 */
		std::thread m_thread;

		/**
		 * @brief Is this {@link WorkerThread} instance stopped ?
		 */
		std::atomic_bool m_stop = false;
	};
} // namespace recpp::async
