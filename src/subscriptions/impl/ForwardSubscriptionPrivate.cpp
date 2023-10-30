export module recpp.subscriptions.impl.ForwardSubscriptionPrivate;

import rscpp.Subscription;

using namespace rscpp;

export namespace recpp
{
	class ForwardSubscriptionPrivate : public Subscription
	{
	public:
		ForwardSubscriptionPrivate(const Subscription &subscription)
			: m_subscription(subscription)
		{
		}

		void request(size_t count) override
		{
			m_subscription.request(count);
		}

		void cancel() override
		{
			m_subscription.cancel();
		}

	private:
		Subscription m_subscription;
	};
} // namespace recpp
