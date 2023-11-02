export module recpp.publishers.Observable;

import rscpp;

import recpp.async.Scheduler;
import recpp.publishers.impl.CreatePublisher;
import recpp.publishers.impl.DeferPublisher;
import recpp.publishers.impl.EmptyPublisher;
import recpp.publishers.impl.ErrorPublisher;
import recpp.publishers.impl.JustPublisher;
import recpp.publishers.impl.NeverPublisher;
import recpp.publishers.impl.RangePublisher;
import recpp.processors.Filter;
import recpp.processors.FlatMap;
import recpp.processors.Map;
import recpp.processors.ObserveOn;
import recpp.processors.SubscribeOn;
import recpp.processors.Tap;
import recpp.subscribers.DefaultSubscriber;
import recpp.subscribers.ObservableSubscriber;

import <functional>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class Observable : public Publisher<T>
	{
		template <typename R>
		friend class Observable;

	public:
		using OnNextMethod = function<void(const T & /* value */)>;
		using OnErrorMethod = function<void(const exception_ptr & /* error */)>;
		using OnCompleteMethod = function<void()>;

		static Observable<T> create(const function<void(ObservableSubscriber<T> &)> &method)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new CreatePublisher<T, ObservableSubscriber<T>>(method)));
		}

		static Observable<T> defer(const function<Observable<T>()> &function)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new DeferPublisher<T, Observable<T>>(function)));
		}

		static Observable<T> empty()
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new EmptyPublisher<T>()));
		}

		static Observable<T> error(const exception_ptr &error)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new ErrorPublisher<T>(error)));
		}

		static Observable<T> just(const T &value)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new JustPublisher<T>(value)));
		}

		static Observable<T> never()
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new NeverPublisher<T>()));
		}

		template <class I>
		static Observable<T> range(I first, I last)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new RangePublisher<T, I>(first, last)));
		}

		template <class R>
		static Observable<T> range(R &&range)
		{
			return Observable<T>::range(range.begin(), range.end());
		}

		void subscribe(const OnNextMethod &onNext, const OnErrorMethod &onError, const OnCompleteMethod &onComplete)
		{
			auto subscriber = DefaultSubscriber<T>(onNext, onError, onComplete);
			Publisher<T>::subscribe(subscriber);
		}

		Observable<T> filter(const function<bool(const T & /* value */)> &method)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new Filter<T>(*this, method)));
		}

		template <typename R>
		Observable<R> map(const function<R(const T & /* value */)> &method)
		{
			return Observable<R>(shared_ptr<Publisher<R>>(new Map<T, R>(*this, method)));
		}

		template <typename R>
		Observable<R> flatMap(const function<Observable<R>(const T & /* value */)> &method)
		{
			return Observable<R>(shared_ptr<Publisher<R>>(new FlatMap<T, R>(*this, method)));
		}

		Observable<T> doOnComplete(const OnCompleteMethod &method)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, nullptr, nullptr, method)));
		}

		Observable<T> doOnError(const OnErrorMethod &method)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, nullptr, method, nullptr)));
		}

		Observable<T> doOnNext(const OnNextMethod &method)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, method, nullptr, nullptr)));
		}

		Observable<T> doOnTerminate(const OnCompleteMethod &method)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new Tap<T>(
				*this, nullptr, [method](const exception_ptr &) { method(); }, method)));
		}

		Observable<T> tap(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, onNextMethod, onErrorMethod, onCompleteMethod)));
		}

		Observable<T> observeOn(Scheduler &scheduler)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new ObserveOn<T>(*this, scheduler)));
		}

		Observable<T> subscribeOn(Scheduler &scheduler)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new SubscribeOn<T>(*this, scheduler)));
		}

	protected:
		Observable(const shared_ptr<Publisher<T>> &dd)
			: Publisher<T>(dd)
		{
		}
	};
} // namespace recpp
