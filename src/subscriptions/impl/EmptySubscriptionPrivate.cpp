export module recpp.subscriptions.impl.EmptySubscriptionPrivate;

import rscpp.Subscriber;
import rscpp.Subscription;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class EmptySubscriptionPrivate : public Subscription
	{
	public:
		EmptySubscriptionPrivate(const Subscriber<T> &subscriber)
			: m_subscriber(subscriber)
		{
		}

		void request(size_t count) override
		{
			m_subscriber.onComplete();
		}

		void cancel() override
		{
			m_subscriber.onComplete();
		}

	private:
		Subscriber<T> m_subscriber;
	};
} // namespace recpp
