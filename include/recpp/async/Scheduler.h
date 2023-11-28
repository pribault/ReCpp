#pragma once

#include <recpp/async/SchedulableQueue.h>

namespace recpp::async
{
	class Schedulable;

	class Scheduler
	{
	public:
		using Clock = SchedulableQueue::Clock;
		using Duration = SchedulableQueue::Duration;
		using TimePoint = SchedulableQueue::TimePoint;

		void schedule(const Schedulable &schedulable);

		void schedule(const TimePoint &timepoint, const Schedulable &schedulable);

		void schedule(const Duration &duration, const Schedulable &schedulable);

	protected:
		SchedulableQueue m_queue;
	};
} // namespace recpp::async
