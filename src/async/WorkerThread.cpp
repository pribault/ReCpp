export module recpp.async.WorkerThread;

import recpp.async.Scheduler;

import <atomic>;
import <thread>;

using namespace std;

export namespace recpp
{
	class WorkerThread : public Scheduler
	{
	public:
		using Clock = Scheduler::Clock;
		using Duration = Scheduler::Duration;
		using TimePoint = Scheduler::TimePoint;

		WorkerThread()
			: m_thread(&WorkerThread::run, this)
		{
		}

		~WorkerThread()
		{
			stop();
		}

		void stop()
		{
			m_stop = true;
			m_queue.stop();
			m_queue.notifyAll();
		}

	private:
		void run()
		{
			while (!m_stop)
			{
				auto schedulable = m_queue.blockingPop();
				if (schedulable)
					schedulable.value()();
			}
		}

		jthread		m_thread;
		atomic_bool m_stop = false;
	};
} // namespace recpp
