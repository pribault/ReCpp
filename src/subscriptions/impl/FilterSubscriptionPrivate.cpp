export module recpp.subscriptions.impl.FilterSubscriptionPrivate;

import rscpp.Subscription;

using namespace rscpp;

export namespace recpp
{
	class FilterSubscriptionPrivate : public Subscription
	{
	public:
		FilterSubscriptionPrivate(const Subscription &subscription)
			: m_subscription(subscription)
		{
		}

		void request(size_t count) override
		{
			m_requested += count;
			m_waiting += count;
			m_subscription.request(count);
		}

		void cancel() override
		{
			m_subscription.cancel();
		}

		void onNext(bool filtered)
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

	private:
		Subscription m_subscription;
		size_t		 m_requested = 0;
		size_t		 m_waiting = 0;
	};
} // namespace recpp
