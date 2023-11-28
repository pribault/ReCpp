#include "recpp/async/Scheduler.h"

using namespace recpp::async;
using namespace std;

void Scheduler::schedule(const Schedulable &schedulable)
{
	schedule(Clock::now(), schedulable);
}

void Scheduler::schedule(const TimePoint &timepoint, const Schedulable &schedulable)
{
	m_queue.push(timepoint, schedulable);
}

void Scheduler::schedule(const Duration &duration, const Schedulable &schedulable)
{
	schedule(Clock::now() + duration, schedulable);
}
