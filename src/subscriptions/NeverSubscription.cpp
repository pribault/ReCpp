export module recpp.subscriptions.NeverSubscription;

import recpp.subscriptions.impl.NeverSubscriptionPrivate;
import rscpp.Subscriber;
import rscpp.Subscription;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class NeverSubscription : public Subscription
	{
	public:
		NeverSubscription(const Subscriber<T> &subscriber)
			: Subscription(shared_ptr<Subscription>(new NeverSubscriptionPrivate<T>(subscriber)))
		{
		}
	};
} // namespace recpp
