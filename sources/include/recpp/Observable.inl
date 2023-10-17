#pragma once

#include <recpp/Subscriber.h>
#include <recpp/Subscription.h>
#include <recpp/subscribers/DefaultSubscriber.h>
#include <recpp/subscriptions/EmptySubscription.h>
#include <recpp/subscriptions/RangeSubscription.h>

template <typename T>
recpp::Observable<T>::Observable(const typename rscpp::Publisher<T>::SubscribeMethod &subscribeMethod)
	: rscpp::Publisher<T>(subscribeMethod)
{
}

template <typename T>
template <typename Function>
recpp::Observable<T> recpp::Observable<T>::create(Function function)
{
	return Observable<T>([function](const rscpp::Subscriber<T> &subscriber) { function(subscriber); });
}

template <typename T>
template <typename Function>
recpp::Observable<T> recpp::Observable<T>::defer(Function function)
{
	return Observable<T>(
		[function](const rscpp::Subscriber<T> &subscriber)
		{
			Observable<T> result = function();
			result.rscpp::Publisher<T>::subscribe(subscriber);
		});
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::empty()
{
	return Observable<T>([](const rscpp::Subscriber<T> &subscriber) { subscriber.onSubscribe(EmptySubscription<T>(subscriber)); });
}

template <typename T>
template <typename Exception>
recpp::Observable<T> recpp::Observable<T>::error(Exception &&exception)
{
	return Observable<T>(
		[exception](const rscpp::Subscriber<T> &subscriber)
		{
			Subscription<T> subscription(
				subscriber, [exception, &subscriber](size_t count) { subscriber.onError(exception); }, nullptr);
			subscriber.onSubscribe(subscription);
		});
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::just(const T &value)
{
	return Observable<T>(
		[value](const rscpp::Subscriber<T> &subscriber)
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

template <typename T>
recpp::Observable<T> recpp::Observable<T>::never()
{
	return Observable<T>(
		[](const rscpp::Subscriber<T> &subscriber)
		{
			Subscription<T> subscription(subscriber, nullptr, nullptr);
			subscriber.onSubscribe(subscription);
		});
}

template <typename T>
template <class I>
recpp::Observable<T> recpp::Observable<T>::range(I first, I last)
{
	return Observable<T>([first, last](const rscpp::Subscriber<T> &subscriber)
								{ subscriber.onSubscribe(RangeSubscription<T, I>(subscriber, first, last)); });
}

template <typename T>
template <class R>
recpp::Observable<T> recpp::Observable<T>::range(R &&range)
{
	return Observable<T>::range(range.begin(), range.end());
}

template <typename T>
template <typename OnNext, typename OnError, typename OnComplete>
void recpp::Observable<T>::subscribe(OnNext onNext, OnError onError, OnComplete onComplete)
{
	rscpp::Publisher<T>::subscribe(DefaultSubscriber<T>(onNext, onError, onComplete));
}
