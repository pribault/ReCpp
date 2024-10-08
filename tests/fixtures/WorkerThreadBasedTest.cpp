#include "fixtures/WorkerThreadBasedTest.h"

// recpp
#include <recpp/async/WorkerThread.h>

using namespace recpp::async;
using namespace std;

WorkerThreadBasedTest::~WorkerThreadBasedTest() = default;

void WorkerThreadBasedTest::SetUp()
{
	m_worker = make_unique<WorkerThread>();
	m_mainThreadId = this_thread::get_id();
	m_workerThreadId = m_worker->threadId();
}

void WorkerThreadBasedTest::TearDown()
{
	m_worker.reset();
	m_mainThreadId = {};
	m_workerThreadId = {};
}
