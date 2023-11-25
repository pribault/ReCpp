#include "recpp/async/Schedulable.h"

using namespace recpp;
using namespace std;

Schedulable::Schedulable(const function<void(void)> &function)
	: m_task(function)
{
}

void Schedulable::operator()()
{
	if (m_task && !m_canceled)
		m_task();
}

void Schedulable::cancel()
{
	m_canceled = true;
}
