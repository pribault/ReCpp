export module recpp.publishers.Single;

import rscpp;

import recpp.async.Scheduler;
import recpp.publishers.impl.CreatePublisher;
import recpp.publishers.impl.DeferPublisher;
import recpp.publishers.impl.ErrorPublisher;
import recpp.publishers.impl.JustPublisher;
import recpp.publishers.impl.NeverPublisher;
import recpp.processors.FlatMap;
import recpp.processors.Map;
import recpp.processors.ObserveOn;
import recpp.processors.SubscribeOn;
import recpp.processors.Tap;
import recpp.subscribers.DefaultSubscriber;
import recpp.subscribers.SingleSubscriber;

import <functional>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class Single : public Publisher<T>
	{
		template <typename R>
		friend class Single;

	public:
		using OnNextMethod = function<void(const T & /* value */)>;
		using OnErrorMethod = function<void(const exception_ptr & /* error */)>;
		using OnCompleteMethod = function<void()>;

		static Single<T> create(const function<void(SingleSubscriber<T> &)> &method)
		{
			return Single<T>(shared_ptr<Publisher<T>>(new CreatePublisher<T, SingleSubscriber<T>>(method)));
		}

		static Single<T> defer(const function<Single<T>()> &function)
		{
			return Single<T>(shared_ptr<Publisher<T>>(new DeferPublisher<T>(function)));
		}

		static Single<T> error(const exception_ptr &error)
		{
			return Single<T>(shared_ptr<Publisher<T>>(new ErrorPublisher<T>(error)));
		}

		static Single<T> just(const T &value)
		{
			return Single<T>(shared_ptr<Publisher<T>>(new JustPublisher<T>(value)));
		}

		static Single<T> never()
		{
			return Single<T>(shared_ptr<Publisher<T>>(new NeverPublisher<T>()));
		}

		void subscribe(const OnNextMethod &onNext, const OnErrorMethod &onError, const OnCompleteMethod &onComplete)
		{
			auto subscriber = DefaultSubscriber<T>(onNext, onError, onComplete);
			Publisher<T>::subscribe(subscriber);
		}

		template <typename R>
		Single<R> map(const function<R(const T & /* value */)> &method)
		{
			return Single<R>(shared_ptr<Publisher<R>>(new Map<T, R>(*this, method)));
		}

		template <typename R>
		Single<R> flatMap(const function<Single<R>(const T & /* value */)> &method)
		{
			return Single<R>(shared_ptr<Publisher<R>>(new FlatMap<T, R>(*this, method)));
		}

		Single<T> doOnComplete(const OnCompleteMethod &method)
		{
			return Single<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, nullptr, nullptr, method)));
		}

		Single<T> doOnError(const OnErrorMethod &method)
		{
			return Single<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, nullptr, method, nullptr)));
		}

		Single<T> doOnNext(const OnNextMethod &method)
		{
			return Single<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, method, nullptr, nullptr)));
		}

		Single<T> doOnTerminate(const OnCompleteMethod &method)
		{
			return Single<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, nullptr, method, method)));
		}

		Single<T> tap(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod)
		{
			return Single<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, onNextMethod, onErrorMethod, onCompleteMethod)));
		}

		Single<T> observeOn(Scheduler &scheduler)
		{
			return Single<T>(shared_ptr<Publisher<T>>(new ObserveOn<T>(*this, scheduler)));
		}

		Single<T> subscribeOn(Scheduler &scheduler)
		{
			return Single<T>(shared_ptr<Publisher<T>>(new SubscribeOn<T>(*this, scheduler)));
		}

	protected:
		Single(const shared_ptr<Publisher<T>> &dd)
			: Publisher<T>(dd)
		{
		}
	};
} // namespace recpp
