export module recpp.subscribers.DefaultSubscriber;

import recpp.subscribers.impl.DefaultSubscriberPrivate;
import rscpp.Subscriber;

import <memory>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class DefaultSubscriber : public Subscriber<T>
	{
	public:
		template <typename N, typename E, typename C>
		explicit DefaultSubscriber(N onNextMethod, E onErrorMethod, C onCompleteMethod)
			: Subscriber<T>(shared_ptr<Subscriber<T>>(new DefaultSubscriberPrivate<T>(onNextMethod, onErrorMethod, onCompleteMethod)))
		{
		}
	};
} // namespace recpp
