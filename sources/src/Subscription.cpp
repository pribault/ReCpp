#include "recpp/Subscription.h"

#include "recpp/Observable.h"

using namespace recpp;

void Subscription::request(size_t count) noexcept
{
	m_requestMethod(count);
}

void Subscription::cancel() noexcept
{
	m_cancelMethod();
}

Subscription::Subscription(const Subscription::RequestMethod &requestMethod, const Subscription::CancelMethod &cancelMethod)
	: m_requestMethod(requestMethod)
	, m_cancelMethod(cancelMethod)
{
}
