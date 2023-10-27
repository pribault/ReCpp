export module recpp.observables.Observable;

import rscpp;

import recpp.subscribers.DefaultSubscriber;
import recpp.subscribers.Subscriber;
import recpp.subscriptions.EmptySubscription;
import recpp.subscriptions.RangeSubscription;
import recpp.subscriptions.Subscription;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class Observable : public Publisher<T>
	{
	public:
		template <typename Function>
		static Observable<T> create(Function function)
		{
			return Observable<T>([function](const auto &subscriber) { function(subscriber); });
		}

		template <typename Function>
		static Observable<T> defer(Function function)
		{
			return Observable<T>(
				[function](const auto &subscriber)
				{
					Observable<T> result = function();
					result.Publisher<T>::subscribe(subscriber);
				});
		}

		static Observable<T> empty()
		{
			return Observable<T>([](const auto &subscriber) { subscriber.onSubscribe(EmptySubscription<T>(subscriber)); });
		}

		template <typename Exception>
		static Observable<T> error(Exception &&exception)
		{
			return Observable<T>(
				[exception](const auto &subscriber)
				{
					Subscription<T> subscription(
						subscriber, [exception, &subscriber](size_t count) { subscriber.onError(exception); }, nullptr);
					subscriber.onSubscribe(subscription);
				});
		}

		static Observable<T> just(const T &value)
		{
			return Observable<T>(
				[value](const auto &subscriber)
				{
					Subscription<T> subscription(
						subscriber,
						[value, &subscriber](size_t count)
						{
							if (count)
							{
								subscriber.onNext(value);
								subscriber.onComplete();
							}
						},
						nullptr);
					subscriber.onSubscribe(subscription);
				});
		}

		static Observable<T> never()
		{
			return Observable<T>(
				[](const auto &subscriber)
				{
					Subscription<T> subscription(subscriber, nullptr, nullptr);
					subscriber.onSubscribe(subscription);
				});
		}

		template <class I>
		static Observable<T> range(I first, I last)
		{
			return Observable<T>([first, last](const auto &subscriber) { subscriber.onSubscribe(RangeSubscription<T, I>(subscriber, first, last)); });
		}

		template <class R>
		static Observable<T> range(R &&range)
		{
			return Observable<T>::range(range.begin(), range.end());
		}

		template <typename OnNext, typename OnError, typename OnComplete>
		void subscribe(OnNext onNext, OnError onError, OnComplete onComplete)
		{
			Publisher<T>::subscribe(DefaultSubscriber<T>(onNext, onError, onComplete));
		}

	protected:
		Observable(const typename Publisher<T>::SubscribeMethod &subscribeMethod)
			: Publisher<T>(subscribeMethod)
		{
		}
	};
} // namespace recpp
