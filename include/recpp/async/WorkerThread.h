#pragma once

#include <recpp/async/Scheduler.h>

#include <atomic>
#include <thread>

namespace recpp
{
	class WorkerThread : public Scheduler
	{
	public:
		WorkerThread();

		~WorkerThread();

		void stop();

	private:
		void run();

		std::thread		 m_thread;
		std::atomic_bool m_stop = false;
	};
} // namespace recpp
