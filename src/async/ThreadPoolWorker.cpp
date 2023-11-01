export module recpp.async.ThreadPoolWorker;

import recpp.async.SchedulableQueue;

import <atomic>;
import <thread>;

using namespace std;

export namespace recpp
{
	class ThreadPoolWorker
	{
	public:
		explicit ThreadPoolWorker(SchedulableQueue &queue)
			: m_queue(queue)
			, m_thread(&ThreadPoolWorker::run, this)
			, m_stop(false)
		{
		}

		~ThreadPoolWorker()
		{
			stop();
		}

		void stop()
		{
			m_stop = true;
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

		SchedulableQueue &m_queue;
		jthread			  m_thread;
		atomic<bool>	  m_stop;
	};
} // namespace recpp
