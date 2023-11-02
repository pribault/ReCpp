export module recpp.subscribers.DefaultSubscriber;

import recpp.subscribers.impl.DefaultSubscriberPrivate;
import rscpp.Subscriber;

import <functional>;
import <memory>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class DefaultSubscriber : public Subscriber<T>
	{
	public:
		using OnNextMethod = function<void(const T & /* value */)>;
		using OnErrorMethod = function<void(const exception_ptr & /* error */)>;
		using OnCompleteMethod = function<void()>;

		explicit DefaultSubscriber(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod)
			: Subscriber<T>(shared_ptr<Subscriber<T>>(new DefaultSubscriberPrivate<T>(onNextMethod, onErrorMethod, onCompleteMethod)))
		{
		}
	};
} // namespace recpp
