export module recpp.async.ThreadPool;

import recpp.async.Scheduler;
import recpp.async.ThreadPoolWorker;

import <memory>;
import <vector>;

using namespace std;

export namespace recpp
{
	class ThreadPool : public Scheduler
	{
		friend class ThreadPoolWorker;

	public:
		explicit ThreadPool(size_t threadCount)
		{
			for (size_t i = 0; i < threadCount; i++)
				m_workers.push_back(make_unique<ThreadPoolWorker>(m_queue));
		}

		~ThreadPool()
		{
			m_queue.stop();
			for (auto &worker : m_workers)
				worker->stop();
			m_queue.notifyAll();
		}

	private:
		vector<unique_ptr<ThreadPoolWorker>> m_workers;
	};
} // namespace recpp
