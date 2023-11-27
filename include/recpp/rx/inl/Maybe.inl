#pragma once

#include <recpp/async/Scheduler.h>
#include <recpp/publishers/CreatePublisher.h>
#include <recpp/publishers/DeferPublisher.h>
#include <recpp/publishers/EmptyPublisher.h>
#include <recpp/publishers/ErrorPublisher.h>
#include <recpp/publishers/JustPublisher.h>
#include <recpp/publishers/NeverPublisher.h>
#include <recpp/processors/FlatMap.h>
#include <recpp/processors/Map.h>
#include <recpp/processors/ObserveOn.h>
#include <recpp/processors/SubscribeOn.h>
#include <recpp/processors/Tap.h>
#include <recpp/subscribers/DefaultSubscriber.h>
#include <recpp/subscribers/MaybeSubscriber.h>

template <typename T>
recpp::Maybe<T> recpp::Maybe<T>::create(const std::function<void(MaybeSubscriber<T> &)> &method)
{
	return Maybe<T>(std::shared_ptr<rscpp::Publisher<T>>(new CreatePublisher<T, MaybeSubscriber<T>>(method)));
}

template <typename T>
recpp::Maybe<T> recpp::Maybe<T>::defer(const std::function<Maybe<T>()> &function)
{
	return Maybe<T>(std::shared_ptr<rscpp::Publisher<T>>(new DeferPublisher<T, Maybe<T>>(function)));
}

template <typename T>
recpp::Maybe<T> recpp::Maybe<T>::empty()
{
	return Maybe<T>(std::shared_ptr<rscpp::Publisher<T>>(new EmptyPublisher<T>()));
}

template <typename T>
recpp::Maybe<T> recpp::Maybe<T>::error(const std::exception_ptr &error)
{
	return Maybe<T>(std::shared_ptr<rscpp::Publisher<T>>(new ErrorPublisher<T>(error)));
}

template <typename T>
recpp::Maybe<T> recpp::Maybe<T>::just(const T &value)
{
	return Maybe<T>(std::shared_ptr<rscpp::Publisher<T>>(new JustPublisher<T>(value)));
}

template <typename T>
recpp::Maybe<T> recpp::Maybe<T>::never()
{
	return Maybe<T>(std::shared_ptr<rscpp::Publisher<T>>(new NeverPublisher<T>()));
}

template <typename T>
void recpp::Maybe<T>::subscribe(const OnNextMethod &onNext, const OnErrorMethod &onError, const OnCompleteMethod &onComplete)
{
	auto subscriber = DefaultSubscriber<T>(onNext, onError, onComplete);
	rscpp::Publisher<T>::subscribe(subscriber);
}

template <typename T>
template <typename R>
recpp::Maybe<R> recpp::Maybe<T>::map(const std::function<R(const T & /* value */)> &method)
{
	return Maybe<R>(std::shared_ptr<rscpp::Publisher<R>>(new Map<T, R>(*this, method)));
}

template <typename T>
template <typename R>
recpp::Maybe<R> recpp::Maybe<T>::flatMap(const std::function<Maybe<R>(const T & /* value */)> &method)
{
	return Maybe<R>(std::shared_ptr<rscpp::Publisher<R>>(new FlatMap<T, R>(*this, method)));
}

template <typename T>
recpp::Maybe<T> recpp::Maybe<T>::doOnComplete(const OnCompleteMethod &method)
{
	return Maybe<T>(std::shared_ptr<rscpp::Publisher<T>>(new Tap<T>(*this, nullptr, nullptr, method)));
}

template <typename T>
recpp::Maybe<T> recpp::Maybe<T>::doOnError(const OnErrorMethod &method)
{
	return Maybe<T>(std::shared_ptr<rscpp::Publisher<T>>(new Tap<T>(*this, nullptr, method, nullptr)));
}

template <typename T>
recpp::Maybe<T> recpp::Maybe<T>::doOnNext(const OnNextMethod &method)
{
	return Maybe<T>(std::shared_ptr<rscpp::Publisher<T>>(new Tap<T>(*this, method, nullptr, nullptr)));
}

template <typename T>
recpp::Maybe<T> recpp::Maybe<T>::doOnTerminate(const OnCompleteMethod &method)
{
	return Maybe<T>(std::shared_ptr<rscpp::Publisher<T>>(new Tap<T>(
		*this, nullptr, [method](const std::exception_ptr &) { method(); }, method)));
}

template <typename T>
recpp::Maybe<T> recpp::Maybe<T>::tap(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod)
{
	return Maybe<T>(std::shared_ptr<rscpp::Publisher<T>>(new Tap<T>(*this, onNextMethod, onErrorMethod, onCompleteMethod)));
}

template <typename T>
recpp::Maybe<T> recpp::Maybe<T>::observeOn(Scheduler &scheduler)
{
	return Maybe<T>(std::shared_ptr<rscpp::Publisher<T>>(new ObserveOn<T>(*this, scheduler)));
}

template <typename T>
recpp::Maybe<T> recpp::Maybe<T>::subscribeOn(Scheduler &scheduler)
{
	return Maybe<T>(std::shared_ptr<rscpp::Publisher<T>>(new SubscribeOn<T>(*this, scheduler)));
}

template <typename T>
recpp::Maybe<T>::Maybe(const std::shared_ptr<rscpp::Publisher<T>> &dd)
	: rscpp::Publisher<T>(dd)
{
}
