#pragma once

#include <recpp/async/Scheduler.h>

#include <memory>
#include <thread>
#include <vector>

namespace recpp::async
{
	class ThreadPoolWorker;

	class ThreadPool : public Scheduler
	{
		friend class ThreadPoolWorker;

	public:
		explicit ThreadPool(size_t threadCount = std::thread::hardware_concurrency());

		~ThreadPool();

		size_t size() const;

	private:
		std::vector<std::unique_ptr<ThreadPoolWorker>> m_workers;
	};
} // namespace recpp::async
