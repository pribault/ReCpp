export module recpp.subscriptions.EmptySubscription;

import recpp.subscriptions.impl.EmptySubscriptionPrivate;
import rscpp.Subscriber;
import rscpp.Subscription;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class EmptySubscription : public Subscription
	{
	public:
		EmptySubscription(const Subscriber<T> &subscriber)
			: Subscription(shared_ptr<Subscription>(new EmptySubscriptionPrivate(subscriber)))
		{
		}
	};
} // namespace recpp
