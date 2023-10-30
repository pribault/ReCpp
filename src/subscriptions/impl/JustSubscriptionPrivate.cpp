export module recpp.subscriptions.impl.JustSubscriptionPrivate;

import rscpp.Subscriber;
import rscpp.Subscription;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class JustSubscriptionPrivate : public Subscription
	{
	public:
		JustSubscriptionPrivate(const Subscriber<T> &subscriber, const T &value)
			: m_subscriber(subscriber)
			, m_value(value)
		{
		}

		void request(size_t count) override
		{
			if (m_completed)
				return;
			m_completed = true;
			m_subscriber.onNext(m_value);
			m_subscriber.onComplete();
		}

		void cancel() override
		{
			m_subscriber.onComplete();
		}

	private:
		Subscriber<T> m_subscriber;
		T			  m_value;
		bool		  m_completed = false;
	};
} // namespace recpp
