#include "recpp/async/SchedulableQueue.h"

#include <algorithm>

using namespace recpp::async;
using namespace std;

SchedulableQueue::SchedulableQueue(const Duration &idleTime)
	: m_idleTime(idleTime)
{
}

void SchedulableQueue::push(const TimePoint &timePoint, const Schedulable &schedulable)
{
	{
		lock_guard<mutex> lock(m_dataMutex);

		if (m_stop)
			return;

		m_schedulables.insert({timePoint, schedulable});
	}

	m_cv.notify_one();
}

optional<Schedulable> SchedulableQueue::pop(const Duration &duration)
{
	return pop(Clock::now() + duration);
}

optional<Schedulable> SchedulableQueue::pop(const TimePoint &until)
{
	wait(
		[this, until]() -> bool
		{
			const auto now = Clock::now();
			return until <= now || m_stop || canPop(now);
		});
	return tryPop(Clock::now());
}

optional<Schedulable> SchedulableQueue::blockingPop()
{
	wait(
		[this]() -> bool
		{
			return m_stop || canPop(Clock::now());
		});
	return tryPop(Clock::now());
}

bool SchedulableQueue::canPop(const TimePoint &timePoint)
{
	lock_guard<mutex> lock(m_dataMutex);

	const auto it = find_if(m_schedulables.begin(), m_schedulables.end(),
							[&timePoint](const auto &pair)
							{
								return pair.first <= timePoint;
							});
	return it != m_schedulables.end();
}

size_t SchedulableQueue::size()
{
	lock_guard<mutex> lock(m_dataMutex);

	return m_schedulables.size();
}

void SchedulableQueue::stop()
{
	lock_guard<mutex> lock(m_dataMutex);

	m_stop = true;
}

void SchedulableQueue::notifyAll()
{
	m_cv.notify_all();
}

optional<Schedulable> SchedulableQueue::tryPop(const TimePoint &timePoint)
{
	lock_guard<mutex> lock(m_dataMutex);

	const auto it = find_if(m_schedulables.begin(), m_schedulables.end(),
							[&timePoint](const auto &pair)
							{
								return pair.first <= timePoint;
							});
	if (it != m_schedulables.end())
	{
		const auto schedulable = it->second;
		m_schedulables.erase(it);
		return schedulable;
	}

	return {};
}

void SchedulableQueue::wait(const function<bool()> &predicate)
{
	unique_lock<mutex> lock(m_cvMutex);
	while (!predicate())
		m_cv.wait_for(lock, m_idleTime);
}
