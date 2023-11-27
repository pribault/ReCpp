#pragma once

#include <recpp/async/Scheduler.h>
#include <recpp/processors/Filter.h>
#include <recpp/processors/FlatMap.h>
#include <recpp/processors/Map.h>
#include <recpp/processors/ObserveOn.h>
#include <recpp/processors/SubscribeOn.h>
#include <recpp/processors/Tap.h>
#include <recpp/publishers/CreatePublisher.h>
#include <recpp/publishers/DeferPublisher.h>
#include <recpp/publishers/EmptyPublisher.h>
#include <recpp/publishers/ErrorPublisher.h>
#include <recpp/publishers/JustPublisher.h>
#include <recpp/publishers/NeverPublisher.h>
#include <recpp/publishers/RangePublisher.h>
#include <recpp/subscribers/DefaultSubscriber.h>
#include <recpp/subscribers/ObservableSubscriber.h>

template <typename T>
recpp::Observable<T> recpp::Observable<T>::create(const std::function<void(ObservableSubscriber<T> &)> &method)
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new CreatePublisher<T, ObservableSubscriber<T>>(method)));
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::defer(const std::function<Observable<T>()> &function)
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new DeferPublisher<T, Observable<T>>(function)));
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::empty()
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new EmptyPublisher<T>()));
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::error(const std::exception_ptr &error)
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new ErrorPublisher<T>(error)));
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::just(const T &value)
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new JustPublisher<T>(value)));
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::never()
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new NeverPublisher<T>()));
}

template <typename T>
template <class I>
recpp::Observable<T> recpp::Observable<T>::range(I first, I last)
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new RangePublisher<T, I>(first, last)));
}

template <typename T>
template <class R>
recpp::Observable<T> recpp::Observable<T>::range(R &&range)
{
	return Observable<T>::range(range.begin(), range.end());
}

template <typename T>
void recpp::Observable<T>::subscribe(const OnNextMethod &onNext, const OnErrorMethod &onError, const OnCompleteMethod &onComplete)
{
	auto subscriber = DefaultSubscriber<T>(onNext, onError, onComplete);
	rscpp::Publisher<T>::subscribe(subscriber);
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::filter(const std::function<bool(const T & /* value */)> &method)
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new Filter<T>(*this, method)));
}

template <typename T>
template <typename R>
recpp::Observable<R> recpp::Observable<T>::map(const std::function<R(const T & /* value */)> &method)
{
	return Observable<R>(std::shared_ptr<rscpp::Publisher<R>>(new Map<T, R>(*this, method)));
}

template <typename T>
template <typename R>
recpp::Observable<R> recpp::Observable<T>::flatMap(const std::function<Observable<R>(const T & /* value */)> &method)
{
	return Observable<R>(std::shared_ptr<rscpp::Publisher<R>>(new FlatMap<T, R>(*this, method)));
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::doOnComplete(const OnCompleteMethod &method)
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new Tap<T>(*this, nullptr, nullptr, method)));
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::doOnError(const OnErrorMethod &method)
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new Tap<T>(*this, nullptr, method, nullptr)));
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::doOnNext(const OnNextMethod &method)
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new Tap<T>(*this, method, nullptr, nullptr)));
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::doOnTerminate(const OnCompleteMethod &method)
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new Tap<T>(
		*this, nullptr, [method](const std::exception_ptr &) { method(); }, method)));
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::tap(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod)
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new Tap<T>(*this, onNextMethod, onErrorMethod, onCompleteMethod)));
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::observeOn(Scheduler &scheduler)
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new ObserveOn<T>(*this, scheduler)));
}

template <typename T>
recpp::Observable<T> recpp::Observable<T>::subscribeOn(Scheduler &scheduler)
{
	return Observable<T>(std::shared_ptr<rscpp::Publisher<T>>(new SubscribeOn<T>(*this, scheduler)));
}

template <typename T>
recpp::Observable<T>::Observable(const std::shared_ptr<rscpp::Publisher<T>> &dd)
	: rscpp::Publisher<T>(dd)
{
}
