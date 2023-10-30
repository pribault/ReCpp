export module recpp.subscriptions.ForwardSubscription;

import recpp.subscriptions.impl.ForwardSubscriptionPrivate;
import rscpp.Subscription;

import <memory>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	class ForwardSubscription : public Subscription
	{
	public:
		ForwardSubscription(const Subscription &subscription)
			: Subscription(shared_ptr<Subscription>(new ForwardSubscriptionPrivate(subscription)))
		{
		}
	};
} // namespace recpp
