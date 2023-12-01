#pragma once

#include <recpp/async/Scheduler.h>
#include <recpp/processors/FlatMap.h>
#include <recpp/processors/IgnoreElements.h>
#include <recpp/processors/Map.h>
#include <recpp/processors/ObserveOn.h>
#include <recpp/processors/SubscribeOn.h>
#include <recpp/processors/Tap.h>
#include <recpp/publishers/CreatePublisher.h>
#include <recpp/publishers/DeferPublisher.h>
#include <recpp/publishers/ErrorPublisher.h>
#include <recpp/publishers/JustPublisher.h>
#include <recpp/publishers/NeverPublisher.h>
#include <recpp/rx/Completable.h>
#include <recpp/subscribers/DefaultSubscriber.h>
#include <recpp/subscribers/SingleSubscriber.h>

template <typename T>
recpp::rx::Single<T> recpp::rx::Single<T>::create(const std::function<void(recpp::subscribers::SingleSubscriber<T> &)> &method)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new recpp::publishers::CreatePublisher<T, recpp::subscribers::SingleSubscriber<T>>(method)));
}

template <typename T>
recpp::rx::Single<T> recpp::rx::Single<T>::defer(const std::function<Single<T>()> &function)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new recpp::publishers::DeferPublisher<T, Single<T>>(function)));
}

template <typename T>
recpp::rx::Single<T> recpp::rx::Single<T>::error(const std::exception_ptr &error)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new recpp::publishers::ErrorPublisher<T>(error)));
}

template <typename T>
recpp::rx::Single<T> recpp::rx::Single<T>::just(const T &value)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new recpp::publishers::JustPublisher<T>(value)));
}

template <typename T>
recpp::rx::Single<T> recpp::rx::Single<T>::never()
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new recpp::publishers::NeverPublisher<T>()));
}

template <typename T>
void recpp::rx::Single<T>::subscribe(const OnSuccessMethod &onSuccess, const OnErrorMethod &onError)
{
	auto subscriber = recpp::subscribers::DefaultSubscriber<T>(onSuccess, onError, nullptr);
	rscpp::Publisher<T>::subscribe(subscriber);
}

template <typename T>
template <typename R>
recpp::rx::Single<R> recpp::rx::Single<T>::map(const std::function<R(const T & /* value */)> &method)
{
	return Single<R>(std::shared_ptr<rscpp::Publisher<R>>(new processors::Map<T, R>(*this, method)));
}

template <typename T>
template <typename R>
recpp::rx::Single<R> recpp::rx::Single<T>::flatMap(const std::function<Single<R>(const T & /* value */)> &method)
{
	return Single<R>(std::shared_ptr<rscpp::Publisher<R>>(new processors::FlatMap<T, R>(*this, method)));
}

template <typename T>
recpp::rx::Completable recpp::rx::Single<T>::ignoreElement()
{
	return Completable(std::shared_ptr<rscpp::Publisher<T>>(new processors::IgnoreElements<int, int>(*this)));
}

template <typename T>
recpp::rx::Single<T> recpp::rx::Single<T>::doOnError(const OnErrorMethod &method)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new processors::Tap<T>(*this, nullptr, method, nullptr)));
}

template <typename T>
recpp::rx::Single<T> recpp::rx::Single<T>::doOnSuccess(const OnSuccessMethod &method)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new processors::Tap<T>(*this, method, nullptr, nullptr)));
}

template <typename T>
recpp::rx::Single<T> recpp::rx::Single<T>::doOnTerminate(const OnCompleteMethod &method)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new processors::Tap<T>(
		*this, nullptr, [method](const std::exception_ptr &) { method(); }, method)));
}

template <typename T>
recpp::rx::Single<T> recpp::rx::Single<T>::tap(const OnSuccessMethod &onSuccessMethod, const OnErrorMethod &onErrorMethod)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new processors::Tap<T>(*this, onSuccessMethod, onErrorMethod, nullptr)));
}

template <typename T>
recpp::rx::Single<T> recpp::rx::Single<T>::observeOn(recpp::async::Scheduler &scheduler)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new processors::ObserveOn<T>(*this, scheduler)));
}

template <typename T>
recpp::rx::Single<T> recpp::rx::Single<T>::subscribeOn(recpp::async::Scheduler &scheduler)
{
	return Single<T>(std::shared_ptr<rscpp::Publisher<T>>(new processors::SubscribeOn<T>(*this, scheduler)));
}

template <typename T>
recpp::rx::Single<T>::Single(const std::shared_ptr<rscpp::Publisher<T>> &dd)
	: rscpp::Publisher<T>(dd)
{
}
