export module recpp.subscriptions.impl.RangeSubscriptionPrivate;

import rscpp.Subscriber;
import rscpp.Subscription;

using namespace rscpp;

export namespace recpp
{
	template <typename T, class I>
	class RangeSubscriptionPrivate : public Subscription
	{
	public:
		RangeSubscriptionPrivate(const Subscriber<T> &subscriber, I first, I last)
			: m_subscriber(subscriber)
			, m_current(first)
			, m_last(last)
		{
		}

		void request(size_t count) override
		{
			for (size_t i = 0; i < count; i++)
			{
				if (m_completed || m_canceled)
					break;
				if (m_current == m_last)
				{
					m_completed = true;
					m_subscriber.onComplete();
					break;
				}
				const auto it = m_current++;
				m_subscriber.onNext(*it);
			}
		}

		void cancel() override
		{
			m_canceled = true;
			m_subscriber.onComplete();
		}

	private:
		Subscriber<T> m_subscriber;
		I			  m_current;
		I			  m_last;
		bool		  m_completed = false;
		bool		  m_canceled = false;
	};
} // namespace recpp
