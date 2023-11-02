export module recpp.publishers.Maybe;

import rscpp;

import recpp.async.Scheduler;
import recpp.publishers.impl.CreatePublisher;
import recpp.publishers.impl.DeferPublisher;
import recpp.publishers.impl.EmptyPublisher;
import recpp.publishers.impl.ErrorPublisher;
import recpp.publishers.impl.JustPublisher;
import recpp.publishers.impl.NeverPublisher;
import recpp.processors.FlatMap;
import recpp.processors.Map;
import recpp.processors.ObserveOn;
import recpp.processors.SubscribeOn;
import recpp.processors.Tap;
import recpp.subscribers.DefaultSubscriber;
import recpp.subscribers.MaybeSubscriber;

import <functional>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class Maybe : public Publisher<T>
	{
		template <typename R>
		friend class Maybe;

	public:
		using OnNextMethod = function<void(const T & /* value */)>;
		using OnErrorMethod = function<void(const exception_ptr & /* error */)>;
		using OnCompleteMethod = function<void()>;

		static Maybe<T> create(const function<void(MaybeSubscriber<T> &)> &method)
		{
			return Maybe<T>(shared_ptr<Publisher<T>>(new CreatePublisher<T, MaybeSubscriber<T>>(method)));
		}

		static Maybe<T> defer(const function<Maybe<T>()> &function)
		{
			return Maybe<T>(shared_ptr<Publisher<T>>(new DeferPublisher<T, Maybe<T>>(function)));
		}

		static Maybe<T> empty()
		{
			return Maybe<T>(shared_ptr<Publisher<T>>(new EmptyPublisher<T>()));
		}

		static Maybe<T> error(const exception_ptr &error)
		{
			return Maybe<T>(shared_ptr<Publisher<T>>(new ErrorPublisher<T>(error)));
		}

		static Maybe<T> just(const T &value)
		{
			return Maybe<T>(shared_ptr<Publisher<T>>(new JustPublisher<T>(value)));
		}

		static Maybe<T> never()
		{
			return Maybe<T>(shared_ptr<Publisher<T>>(new NeverPublisher<T>()));
		}

		void subscribe(const OnNextMethod &onNext, const OnErrorMethod &onError, const OnCompleteMethod &onComplete)
		{
			auto subscriber = DefaultSubscriber<T>(onNext, onError, onComplete);
			Publisher<T>::subscribe(subscriber);
		}

		template <typename R>
		Maybe<R> map(const function<R(const T & /* value */)> &method)
		{
			return Maybe<R>(shared_ptr<Publisher<R>>(new Map<T, R>(*this, method)));
		}

		template <typename R>
		Maybe<R> flatMap(const function<Maybe<R>(const T & /* value */)> &method)
		{
			return Maybe<R>(shared_ptr<Publisher<R>>(new FlatMap<T, R>(*this, method)));
		}

		Maybe<T> doOnComplete(const OnCompleteMethod &method)
		{
			return Maybe<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, nullptr, nullptr, method)));
		}

		Maybe<T> doOnError(const OnErrorMethod &method)
		{
			return Maybe<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, nullptr, method, nullptr)));
		}

		Maybe<T> doOnNext(const OnNextMethod &method)
		{
			return Maybe<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, method, nullptr, nullptr)));
		}

		Maybe<T> doOnTerminate(const OnCompleteMethod &method)
		{
			return Maybe<T>(shared_ptr<Publisher<T>>(new Tap<T>(
				*this, nullptr, [method](const exception_ptr &) { method(); }, method)));
		}

		Maybe<T> tap(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod)
		{
			return Maybe<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, onNextMethod, onErrorMethod, onCompleteMethod)));
		}

		Maybe<T> observeOn(Scheduler &scheduler)
		{
			return Maybe<T>(shared_ptr<Publisher<T>>(new ObserveOn<T>(*this, scheduler)));
		}

		Maybe<T> subscribeOn(Scheduler &scheduler)
		{
			return Maybe<T>(shared_ptr<Publisher<T>>(new SubscribeOn<T>(*this, scheduler)));
		}

	protected:
		Maybe(const shared_ptr<Publisher<T>> &dd)
			: Publisher<T>(dd)
		{
		}
	};
} // namespace recpp
