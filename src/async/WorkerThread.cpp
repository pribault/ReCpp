#include "recpp/async/WorkerThread.h"

using namespace recpp::async;
using namespace std;

WorkerThread::WorkerThread()
	: m_thread(&WorkerThread::run, this)
{
}

WorkerThread::~WorkerThread()
{
	stop();
	m_thread.join();
}

void WorkerThread::stop()
{
	m_stop = true;
	m_queue.stop();
	m_queue.notifyAll();
}

thread::id WorkerThread::threadId() const
{
	return m_thread.get_id();
}

void WorkerThread::run()
{
	while (!m_stop)
	{
		while (m_queue.canPop(SchedulableQueue::Clock::now()))
		{
			auto schedulable = m_queue.blockingPop();
			if (schedulable)
				schedulable.value()();
		}
	}
}
