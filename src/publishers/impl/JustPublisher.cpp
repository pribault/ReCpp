export module recpp.publishers.impl.JustPublisher;

import rscpp;
import recpp.subscriptions.JustSubscription;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class JustPublisher : public Publisher<T>
	{
	public:
		JustPublisher(const T &value)
			: m_value(value)
		{
		}

		void subscribe(Subscriber<T> &subscriber) override
		{
			auto subscription = JustSubscription<T>(subscriber, m_value);
			subscriber.onSubscribe(subscription);
		}

	private:
		T m_value;
	};
} // namespace recpp
