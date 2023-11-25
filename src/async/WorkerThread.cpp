#include "recpp/async/WorkerThread.h"

using namespace recpp;
using namespace std;

WorkerThread::WorkerThread()
	: m_thread(&WorkerThread::run, this)
{
}

WorkerThread::~WorkerThread()
{
	stop();
}

void WorkerThread::stop()
{
	m_stop = true;
	m_queue.stop();
	m_queue.notifyAll();
}

void WorkerThread::run()
{
	while (!m_stop)
	{
		auto schedulable = m_queue.blockingPop();
		if (schedulable)
			schedulable.value()();
	}
}
