#pragma once

#include <recpp/Subscriber.h>
#include <recpp/Subscription.h>

template <typename T>
recpp::Observable<T>::Observable(const SubscribeMethod &subscribeMethod)
	: m_subscribeMethod(subscribeMethod)
{
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::empty()
{
	return recpp::Observable<T>(
		[](rscpp::Subscriber<T> &subscriber)
		{
			recpp::Subscription subscription(
				[&subscriber](size_t count)
				{
					if (count)
					{
						subscriber.onComplete();
					}
				},
				[&subscriber]() { subscriber.onComplete(); });
			subscriber.onSubscribe(subscription);
		});
}

template <typename T>
template <typename Exception>
recpp::Observable<T> recpp::Observable<T>::error(Exception &&exception)
{
	return recpp::Observable<T>(
		[exception](rscpp::Subscriber<T> &subscriber)
		{
			recpp::Subscription subscription([exception, &subscriber](size_t count) { subscriber.onError(exception); },
											 [exception, &subscriber]() { subscriber.onError(exception); });
			subscriber.onSubscribe(subscription);
		});
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::just(const T &value)
{
	return recpp::Observable<T>(
		[&value](rscpp::Subscriber<T> &subscriber)
		{
			recpp::Subscription subscription(
				[&value, &subscriber](size_t count)
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
void recpp::Observable<T>::subscribe(rscpp::Subscriber<T> &subscriber) noexcept
{
	m_subscribeMethod(subscriber);
}

template <typename T>
template <typename OnNext, typename OnError, typename OnComplete>
void recpp::Observable<T>::subscribe(OnNext onNext, OnError onError, OnComplete onComplete)
{
	m_subscribeMethod(recpp::Subscriber<T>(onNext, onError, onComplete));
}
