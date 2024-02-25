#include "recpp/async/EventLoop.h"

using namespace recpp::async;
using namespace std;

void EventLoop::run()
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

void EventLoop::runFor(const Duration &duration)
{
	const auto start = Clock::now();

	while (!m_stop)
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

void EventLoop::stop()
{
	m_queue.stop();
	m_queue.notifyAll();
	m_stop = true;
}
