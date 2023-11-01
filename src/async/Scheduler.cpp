export module recpp.async.Scheduler;

import recpp.async.Schedulable;
import recpp.async.SchedulableQueue;

using namespace std;

export namespace recpp
{
	class Scheduler
	{
	public:
		using Clock = SchedulableQueue::Clock;
		using Duration = SchedulableQueue::Duration;
		using TimePoint = SchedulableQueue::TimePoint;

		void schedule(const Schedulable &schedulable)
		{
			schedule(Clock::now(), schedulable);
		}
		void schedule(const TimePoint &timepoint, const Schedulable &schedulable)
		{
			m_queue.push(timepoint, schedulable);
		}
		void schedule(const Duration &duration, const Schedulable &schedulable)
		{
			schedule(Clock::now() + duration, schedulable);
		}

	protected:
		SchedulableQueue m_queue;
	};
} // namespace recpp
