#include "recpp/async/ThreadPoolWorker.h"

#include "recpp/async/SchedulableQueue.h"

using namespace recpp::async;
using namespace std;

ThreadPoolWorker::ThreadPoolWorker(SchedulableQueue &queue)
	: m_queue(queue)
	, m_thread(&ThreadPoolWorker::run, this)
	, m_stop(false)
{
}

ThreadPoolWorker::~ThreadPoolWorker()
{
	stop();
	m_thread.join();
}

void ThreadPoolWorker::stop()
{
	m_stop = true;
}

void ThreadPoolWorker::run()
{
	while (!m_stop)
	{
		auto schedulable = m_queue.blockingPop();
		if (schedulable)
			schedulable.value()();
	}
}
