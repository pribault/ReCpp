#pragma once

#include <atomic>
#include <thread>

namespace recpp
{
	class SchedulableQueue;

	class ThreadPoolWorker
	{
	public:
		explicit ThreadPoolWorker(SchedulableQueue &queue);

		~ThreadPoolWorker();

		void stop();

	private:
		void run();

		SchedulableQueue &m_queue;
		std::thread		  m_thread;
		std::atomic<bool> m_stop;
	};
} // namespace recpp
