export module recpp.observables.impl.JustObservable;

import rscpp;
import recpp.subscriptions.JustSubscription;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class JustObservable : public Publisher<T>
	{
	public:
		JustObservable(const T &value)
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
