export module recpp.subscriptions.RangeSubscription;

import recpp.subscriptions.impl.RangeSubscriptionPrivate;
import rscpp.Subscriber;
import rscpp.Subscription;

import <memory>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T, class I>
	class RangeSubscription : public Subscription
	{
	public:
		RangeSubscription(const Subscriber<T> &subscriber, I first, I last)
			: Subscription(shared_ptr<Subscription>(new RangeSubscriptionPrivate(subscriber, first, last)))
		{
		}
	};
} // namespace recpp
