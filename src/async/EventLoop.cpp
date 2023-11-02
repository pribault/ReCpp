export module recpp.async.EventLoop;

import recpp.async.Scheduler;

import <atomic>;

using namespace std;

export namespace recpp
{
	class EventLoop : public Scheduler
	{
	public:
		using Clock = Scheduler::Clock;
		using Duration = Scheduler::Duration;
		using TimePoint = Scheduler::TimePoint;

		void run()
		{
			while (!m_stop.load())
			{
				auto schedulable = m_queue.blockingPop();
				if (schedulable)
					schedulable.value()();
			}
		}

		void runFor(const Duration &duration)
		{
			const auto start = Clock::now();

			while (!m_stop.load())
			{
				const auto now = Clock::now();
				const auto diff = now - start;
				if (diff >= duration)
					break;

				auto schedulable = m_queue.pop(duration - diff);
				if (!schedulable)
					break;

				schedulable.value()();
			}
		}

		void stop()
		{
			m_stop = true;
			m_queue.stop();
			m_queue.notifyAll();
		}

	private:
		atomic_bool m_stop = false;
	};
} // namespace recpp
