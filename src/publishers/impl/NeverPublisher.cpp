export module recpp.publishers.impl.NeverPublisher;

import recpp.subscriptions.NeverSubscription;
import rscpp;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class NeverPublisher : public Publisher<T>
	{
	public:
		NeverPublisher() {}

		void subscribe(Subscriber<T> &subscriber) override
		{
			auto subscription = NeverSubscription<T>(subscriber);
			subscriber.onSubscribe(subscription);
		}
	};
} // namespace recpp
