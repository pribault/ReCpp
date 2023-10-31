export module recpp.publishers.Observable;

import rscpp;

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
import recpp.processors.Tap;
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
		template <typename R>
		friend class Observable;

	public:
		template <typename F>
		static Observable<T> create(F function)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new CreatePublisher<T, F>(function)));
		}

		template <typename F>
		static Observable<T> defer(F function)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new DeferPublisher<T, F>(function)));
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

		template <typename OnNext, typename OnError, typename OnComplete>
		void subscribe(OnNext onNext, OnError onError, OnComplete onComplete)
		{
			auto subscriber = DefaultSubscriber<T>(onNext, onError, onComplete);
			Publisher<T>::subscribe(subscriber);
		}

		template <typename R, typename M>
		Observable<R> filter(M method)
		{
			return Observable<R>(shared_ptr<Publisher<R>>(new Filter<T, R>(*this, method)));
		}

		template <typename R, typename M>
		Observable<R> map(M method)
		{
			return Observable<R>(shared_ptr<Publisher<R>>(new Map<T, R>(*this, method)));
		}

		template <typename R, typename M>
		Observable<R> flatMap(M method)
		{
			return Observable<R>(shared_ptr<Publisher<R>>(new FlatMap<T, R>(*this, method)));
		}

		template <typename C>
		Observable<T> doOnComplete(C method)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, nullptr, nullptr, method)));
		}

		template <typename E>
		Observable<T> doOnError(E method)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, nullptr, method, nullptr)));
		}

		template <typename N>
		Observable<T> doOnNext(N method)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, method, nullptr, nullptr)));
		}

		template <typename T>
		Observable<T> doOnTerminate(T method)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, nullptr, method, method)));
		}

		template <typename N, typename E, typename C>
		Observable<T> tap(N onNextMethod, E onErrorMethod, C onCompleteMethod)
		{
			return Observable<T>(shared_ptr<Publisher<T>>(new Tap<T>(*this, onNextMethod, onErrorMethod, onCompleteMethod)));
		}

	protected:
		Observable(const shared_ptr<Publisher<T>> &dd)
			: Publisher<T>(dd)
		{
		}
	};
} // namespace recpp
