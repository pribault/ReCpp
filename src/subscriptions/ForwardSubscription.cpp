#include "recpp/subscriptions/ForwardSubscription.h"

using namespace recpp;
using namespace rscpp;
using namespace std;

ForwardSubscription::ForwardSubscription(const Subscription &subscription)
	: Subscription(shared_ptr<Subscription>(new Impl(subscription)))
{
}

ForwardSubscription::Impl::Impl(const Subscription &subscription)
	: m_subscription(subscription)
{
}

void ForwardSubscription::Impl::request(size_t count)
{
	m_subscription.request(count);
}

void ForwardSubscription::Impl::cancel()
{
	m_subscription.cancel();
}
