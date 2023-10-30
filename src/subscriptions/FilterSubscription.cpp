export module recpp.subscriptions.FilterSubscription;

import recpp.subscriptions.impl.FilterSubscriptionPrivate;
import rscpp.Subscription;

import <memory>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	class FilterSubscription : public Subscription
	{
	public:
		FilterSubscription(const Subscription &subscription)
			: Subscription(shared_ptr<Subscription>(new FilterSubscriptionPrivate(subscription)))
		{
		}

		void onNext(bool filtered)
		{
			static_cast<FilterSubscriptionPrivate *>(d_ptr.get())->onNext(filtered);
		}
	};
} // namespace recpp
