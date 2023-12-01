#pragma once

#include <recpp/async/Scheduler.h>

#include <atomic>

namespace recpp::async
{
	class EventLoop : public Scheduler
	{
	public:
		using Clock = Scheduler::Clock;
		using Duration = Scheduler::Duration;
		using TimePoint = Scheduler::TimePoint;

		void run();

		void runFor(const Duration &duration);

		void stop();

	private:
		std::atomic_bool m_stop = false;
	};
} // namespace recpp::async
