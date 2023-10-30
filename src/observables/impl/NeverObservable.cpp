export module recpp.observables.impl.NeverObservable;

import recpp.subscriptions.NeverSubscription;
import rscpp;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class NeverObservable : public Publisher<T>
	{
	public:
		NeverObservable() {}

		void subscribe(Subscriber<T> &subscriber) override
		{
			auto subscription = NeverSubscription<T>(subscriber);
			subscriber.onSubscribe(subscription);
		}
	};
} // namespace recpp
