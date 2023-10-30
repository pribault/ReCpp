export module recpp.subscriptions.JustSubscription;

import recpp.subscriptions.impl.JustSubscriptionPrivate;
import rscpp.Subscriber;
import rscpp.Subscription;

import <memory>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class JustSubscription : public Subscription
	{
	public:
		JustSubscription(const Subscriber<T> &subscriber, const T &value)
			: Subscription(shared_ptr<Subscription>(new JustSubscriptionPrivate(subscriber, value)))
		{
		}
	};
} // namespace recpp
