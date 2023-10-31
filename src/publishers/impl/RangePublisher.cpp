export module recpp.publishers.impl.RangePublisher;

import rscpp;
import recpp.subscriptions.RangeSubscription;

using namespace rscpp;

export namespace recpp
{
	template <typename T, class I>
	class RangePublisher : public Publisher<T>
	{
	public:
		RangePublisher(I first, I last)
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
