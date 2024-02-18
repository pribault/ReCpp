#include "recpp/subscriptions/FilterSubscription.h"

using namespace recpp::subscriptions;
using namespace rscpp;
using namespace std;

FilterSubscription::FilterSubscription(const Subscription &subscription)
	: Subscription(std::make_shared<Impl>(subscription))
{
}

void FilterSubscription::onNext(bool filtered)
{
	static_cast<Impl *>(d_ptr.get())->onNext(filtered);
}

FilterSubscription::Impl::Impl(const Subscription &subscription)
	: m_subscription(subscription)
{
}

void FilterSubscription::Impl::request(size_t count)
{
	m_requested += count;
	m_waiting += count;
	m_subscription.request(count);
}

void FilterSubscription::Impl::cancel()
{
	m_subscription.cancel();
}

void FilterSubscription::Impl::onNext(bool filtered)
{
	m_waiting--;
	if (!filtered)
		m_requested--;
	if (!m_waiting && m_requested)
	{
		m_waiting = m_requested;
		m_subscription.request(m_waiting);
	}
}
