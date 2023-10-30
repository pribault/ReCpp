export module recpp.observables.impl.RangeObservable;

import rscpp;
import recpp.subscriptions.RangeSubscription;

using namespace rscpp;

export namespace recpp
{
	template <typename T, class I>
	class RangeObservable : public Publisher<T>
	{
	public:
		RangeObservable(I first, I last)
			: m_first(first)
			, m_last(last)
		{
		}

		void subscribe(Subscriber<T> &subscriber) override
		{
			auto subscription = RangeSubscription<T, I>(subscriber, m_first, m_last);
			subscriber.onSubscribe(subscription);
		}

	private:
		I m_first;
		I m_last;
	};
} // namespace recpp
