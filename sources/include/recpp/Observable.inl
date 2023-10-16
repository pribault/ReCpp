#pragma once

#include <recpp/Subscriber.h>
#include <recpp/Subscription.h>
#include <recpp/subscriptions/EmptySubscription.h>

template <typename T>
recpp::Observable<T>::Observable(const typename rscpp::Publisher<T>::SubscribeMethod &subscribeMethod)
	: rscpp::Publisher<T>(subscribeMethod)
{
}

template <typename T>
template <typename Function>
recpp::Observable<T> recpp::Observable<T>::create(Function function)
{
	return recpp::Observable<T>([function](const rscpp::Subscriber<T> &subscriber) { function(subscriber); });
}

template <typename T>
template <typename Function>
recpp::Observable<T> recpp::Observable<T>::defer(Function function)
{
	return recpp::Observable<T>(
		[function](const rscpp::Subscriber<T> &subscriber)
		{
			recpp::Observable<T> result = function();
			result.rscpp::Publisher<T>::subscribe(subscriber);
		});
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::empty()
{
	return recpp::Observable<T>([](const rscpp::Subscriber<T> &subscriber) { subscriber.onSubscribe(recpp::EmptySubscription<T>(subscriber)); });
}

template <typename T>
template <typename Exception>
recpp::Observable<T> recpp::Observable<T>::error(Exception &&exception)
{
	return recpp::Observable<T>(
		[exception](const rscpp::Subscriber<T> &subscriber)
		{
			recpp::Subscription<T> subscription(
				subscriber, [exception, &subscriber](size_t count) { subscriber.onError(exception); },
				[exception, &subscriber]() { subscriber.onError(exception); });
			subscriber.onSubscribe(subscription);
		});
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::just(const T &value)
{
	return recpp::Observable<T>(
		[value](const rscpp::Subscriber<T> &subscriber)
		{
			recpp::Subscription<T> subscription(
				subscriber,
				[value, &subscriber](size_t count)
				{
					if (count)
					{
						subscriber.onNext(value);
						subscriber.onComplete();
					}
				},
				[&subscriber]() { subscriber.onComplete(); });
			subscriber.onSubscribe(subscription);
		});
}

template <typename T>
template <typename OnNext, typename OnError, typename OnComplete>
void recpp::Observable<T>::subscribe(OnNext onNext, OnError onError, OnComplete onComplete)
{
	rscpp::Publisher<T>::subscribe(recpp::Subscriber<T>([](const rscpp::Subscription &subscription) { subscription.request(1); }, onNext, onError, onComplete));
}
