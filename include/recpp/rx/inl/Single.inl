#pragma once

#include <recpp/async/Scheduler.h>
#include <recpp/publishers/CreatePublisher.h>
#include <recpp/publishers/DeferPublisher.h>
#include <recpp/publishers/ErrorPublisher.h>
#include <recpp/publishers/JustPublisher.h>
#include <recpp/publishers/NeverPublisher.h>
#include <recpp/processors/FlatMap.h>
#include <recpp/processors/Map.h>
#include <recpp/processors/ObserveOn.h>
#include <recpp/processors/SubscribeOn.h>
#include <recpp/processors/Tap.h>
#include <recpp/subscribers/DefaultSubscriber.h>
#include <recpp/subscribers/SingleSubscriber.h>

template <typename T>
recpp::Single<T> recpp::Single<T>::create(const std::function<void(SingleSubscriber<T> &)> &method)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new CreatePublisher<T, SingleSubscriber<T>>(method)));
}

template <typename T>
recpp::Single<T> recpp::Single<T>::defer(const std::function<Single<T>()> &function)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new DeferPublisher<T, Single<T>>(function)));
}

template <typename T>
recpp::Single<T> recpp::Single<T>::error(const std::exception_ptr &error)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new ErrorPublisher<T>(error)));
}

template <typename T>
recpp::Single<T> recpp::Single<T>::just(const T &value)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new JustPublisher<T>(value)));
}

template <typename T>
recpp::Single<T> recpp::Single<T>::never()
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new NeverPublisher<T>()));
}

template <typename T>
void recpp::Single<T>::subscribe(const OnSuccessMethod &onSuccess, const OnErrorMethod &onError)
{
	auto subscriber = DefaultSubscriber<T>(onSuccess, onError, nullptr);
	rscpp::Publisher<T>::subscribe(subscriber);
}

template <typename T>
template <typename R>
recpp::Single<R> recpp::Single<T>::map(const std::function<R(const T & /* value */)> &method)
{
	return Single<R>(std::shared_ptr<rscpp::Publisher<R>>(new Map<T, R>(*this, method)));
}

template <typename T>
template <typename R>
recpp::Single<R> recpp::Single<T>::flatMap(const std::function<Single<R>(const T & /* value */)> &method)
{
	return Single<R>(std::shared_ptr<rscpp::Publisher<R>>(new FlatMap<T, R>(*this, method)));
}

template <typename T>
recpp::Single<T> recpp::Single<T>::doOnError(const OnErrorMethod &method)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new Tap<T>(*this, nullptr, method, nullptr)));
}

template <typename T>
recpp::Single<T> recpp::Single<T>::doOnSuccess(const OnSuccessMethod &method)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new Tap<T>(*this, method, nullptr, nullptr)));
}

template <typename T>
recpp::Single<T> recpp::Single<T>::doOnTerminate(const OnCompleteMethod &method)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new Tap<T>(
		*this, nullptr, [method](const std::exception_ptr &) { method(); }, method)));
}

template <typename T>
recpp::Single<T> recpp::Single<T>::tap(const OnSuccessMethod &onSuccessMethod, const OnErrorMethod &onErrorMethod)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new Tap<T>(*this, onSuccessMethod, onErrorMethod, nullptr)));
}

template <typename T>
recpp::Single<T> recpp::Single<T>::observeOn(Scheduler &scheduler)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new ObserveOn<T>(*this, scheduler)));
}

template <typename T>
recpp::Single<T> recpp::Single<T>::subscribeOn(Scheduler &scheduler)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new SubscribeOn<T>(*this, scheduler)));
}

template <typename T>
recpp::Single<T>::Single(const std::shared_ptr<rscpp::Publisher<T>> &dd)
	: rscpp::Publisher<T>(dd)
{
}
