export module recpp.observables.Observable;

import rscpp;

import recpp.observables.impl.CreateObservable;
import recpp.observables.impl.DeferObservable;
import recpp.observables.impl.EmptyObservable;
import recpp.observables.impl.ErrorObservable;
import recpp.observables.impl.JustObservable;
import recpp.observables.impl.NeverObservable;
import recpp.observables.impl.ProcessedObservable;
import recpp.observables.impl.RangeObservable;
import recpp.processors.Map;
import recpp.subscribers.DefaultSubscriber;
import recpp.subscriptions.EmptySubscription;

import <functional>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class Observable : public Publisher<T>
	{
	public:
		Observable(const shared_ptr<Publisher<T>> &dd)
			: Publisher<T>(dd)
		{
		}

		template <typename F>
		static Observable<T> create(F function)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new CreateObservable<T, F>(function)));
		}

		template <typename F>
		static Observable<T> defer(F function)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new DeferObservable<T, F>(function)));
		}

		static Observable<T> empty()
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new EmptyObservable<T>()));
		}

		static Observable<T> error(const exception_ptr &error)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new ErrorObservable<T>(error)));
		}

		static Observable<T> just(const T &value)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new JustObservable<T>(value)));
		}

		static Observable<T> never()
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new NeverObservable<T>()));
		}

		template <class I>
		static Observable<T> range(I first, I last)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new RangeObservable<T, I>(first, last)));
		}

		template <class R>
		static Observable<T> range(R &&range)
		{
			return Observable<T>::range(range.begin(), range.end());
		}

		template <typename OnNext, typename OnError, typename OnComplete>
		void subscribe(OnNext onNext, OnError onError, OnComplete onComplete)
		{
			auto subscriber = DefaultSubscriber<T>(onNext, onError, onComplete);
			Publisher<T>::subscribe(subscriber);
		}

		template <typename R>
		Observable<R> operator|(const Processor<T, R> &processor)
		{
			return Observable<R>(shared_ptr<Publisher<R>>(new ProcessedObservable<T, R>(processor)));
		}

		template <typename R>
		Observable<R> map(auto method)
		{
			return *this | Map<T, R>(*this, method);
		}
	};
} // namespace recpp
