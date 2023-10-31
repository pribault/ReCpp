export module recpp.publishers.impl.EmptyPublisher;

import recpp.subscriptions.EmptySubscription;
import rscpp;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class EmptyPublisher : public Publisher<T>
	{
	public:
		EmptyPublisher() {}

		void subscribe(Subscriber<T> &subscriber) override
		{
			auto subscription = EmptySubscription<T>(subscriber);
			subscriber.onSubscribe(subscription);
		}
	};
} // namespace recpp
