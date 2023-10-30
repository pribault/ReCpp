export module recpp.subscriptions.impl.NeverSubscriptionPrivate;

import rscpp.Subscriber;
import rscpp.Subscription;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class NeverSubscriptionPrivate : public Subscription
	{
	public:
		NeverSubscriptionPrivate(const Subscriber<T> &subscriber)
			: m_subscriber(subscriber)
		{
		}

		void request(size_t count) override {}

		void cancel() override
		{
			m_subscriber.onComplete();
		}

	private:
		Subscriber<T> m_subscriber;
	};
} // namespace recpp
