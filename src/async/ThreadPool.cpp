#include "recpp/async/ThreadPool.h"

#include "recpp/async/ThreadPoolWorker.h"

#include <thread>

using namespace recpp::async;
using namespace std;

ThreadPool::ThreadPool(size_t threadCount)
{
	for (size_t i = 0; i < threadCount; i++)
		m_workers.push_back(make_unique<ThreadPoolWorker>(m_queue));
}

ThreadPool::~ThreadPool()
{
	m_queue.stop();
	for (auto &worker : m_workers)
		worker->stop();
	m_queue.notifyAll();
}

size_t ThreadPool::size() const
{
	return m_workers.size();
}
