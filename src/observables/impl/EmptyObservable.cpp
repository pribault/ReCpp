export module recpp.observables.impl.EmptyObservable;

import recpp.subscriptions.EmptySubscription;
import rscpp;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class EmptyObservable : public Publisher<T>
	{
	public:
		EmptyObservable() {}

		void subscribe(Subscriber<T> &subscriber) override
		{
			auto subscription = EmptySubscription<T>(subscriber);
			subscriber.onSubscribe(subscription);
		}
	};
} // namespace recpp
