#pragma once

#include <recpp/async/Scheduler.h>

#include <memory>
#include <thread>
#include <vector>

namespace recpp::async
{
	class ThreadPoolWorker;

	/**
	 * @class ThreadPool ThreadPool.h <recpp/async/ThreadPool.h>
	 * @brief Represents a thread pool {@link Scheduler} you can use to run performance intensive tasks.
	 * <p>
	 * When scheduling a {@link Schedulable} on a {@link ThreadPool}, the first {@link ThreadPoolWorker} available will run it.
	 */
	class ThreadPool : public Scheduler
	{
		friend class ThreadPoolWorker;

	public:
		/**
		 * @brief Construct a new {@link ThreadPool} instance with the given worker threads count.
		 * <p>
		 * By default the number of cpu cores will be taken.
		 *
		 * @param threadCount The number worker threads to create.
		 */
		explicit ThreadPool(size_t threadCount = std::thread::hardware_concurrency());

		/**
		 * @brief Destroy this {@link ThreadPool} instance
		 */
		~ThreadPool();

		/**
		 * @brief Get the worker threads count
		 *
		 * @return size_t The worker threads count
		 */
		size_t size() const;

	private:
		/**
		 * @brief The worker threads list
		 */
		std::vector<std::unique_ptr<ThreadPoolWorker>> m_workers;
	};
} // namespace recpp::async
