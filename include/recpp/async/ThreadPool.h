#pragma once

#include <recpp/async/Scheduler.h>

#include <memory>
#include <vector>

namespace recpp::async
{
	class ThreadPoolWorker;

	class ThreadPool : public Scheduler
	{
		friend class ThreadPoolWorker;

	public:
		explicit ThreadPool(size_t threadCount);

		~ThreadPool();

	private:
		std::vector<std::unique_ptr<ThreadPoolWorker>> m_workers;
	};
} // namespace recpp::async
