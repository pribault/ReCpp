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
#include <recpp/publishers/EmptyPublisher.h>
#include <recpp/publishers/ErrorPublisher.h>
#include <recpp/publishers/JustPublisher.h>
#include <recpp/publishers/NeverPublisher.h>
#include <recpp/rx/Completable.h>
#include <recpp/subscribers/DefaultSubscriber.h>
#include <recpp/subscribers/MaybeSubscriber.h>

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Maybe<T>::create(const std::function<void(recpp::subscribers::MaybeSubscriber<T> &)> &method)
{
	return Maybe<T>(std::make_shared<recpp::publishers::CreatePublisher<T, recpp::subscribers::MaybeSubscriber<T>>>(method));
}

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Maybe<T>::defer(const std::function<Maybe<T>()> &function)
{
	return Maybe<T>(std::make_shared<recpp::publishers::DeferPublisher<T, Maybe<T>>>(function));
}

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Maybe<T>::empty()
{
	return Maybe<T>(std::make_shared<recpp::publishers::EmptyPublisher<T>>());
}

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Maybe<T>::error(const std::exception_ptr &error)
{
	return Maybe<T>(std::make_shared<recpp::publishers::ErrorPublisher<T>>(error));
}

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Maybe<T>::just(const T &value)
{
	return Maybe<T>(std::make_shared<recpp::publishers::JustPublisher<T>>(value));
}

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Maybe<T>::never()
{
	return Maybe<T>(std::make_shared<recpp::publishers::NeverPublisher<T>>());
}

template <typename T>
void recpp::rx::Maybe<T>::subscribe(const OnNextMethod &onNext, const OnErrorMethod &onError, const OnCompleteMethod &onComplete)
{
	auto subscriber = recpp::subscribers::DefaultSubscriber<T>(onNext, onError, onComplete);
	rscpp::Publisher<T>::subscribe(subscriber);
}

template <typename T>
template <typename R>
recpp::rx::Maybe<R> recpp::rx::Maybe<T>::map(const std::function<R(const T & /* value */)> &method)
{
	return Maybe<R>(std::make_shared<processors::Map<T, R>>(*this, method));
}

template <typename T>
template <typename R>
recpp::rx::Maybe<R> recpp::rx::Maybe<T>::flatMap(const std::function<Maybe<R>(const T & /* value */)> &method)
{
	return Maybe<R>(std::make_shared<processors::FlatMap<T, R>>(*this, method));
}

template <typename T>
recpp::rx::Completable recpp::rx::Maybe<T>::ignoreElement()
{
	return Completable(std::make_shared<processors::IgnoreElements<int, int>>(*this));
}

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Maybe<T>::doOnComplete(const OnCompleteMethod &method)
{
	return Maybe<T>(std::make_shared<processors::Tap<T>>(*this, nullptr, nullptr, method));
}

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Maybe<T>::doOnError(const OnErrorMethod &method)
{
	return Maybe<T>(std::make_shared<processors::Tap<T>>(*this, nullptr, method, nullptr));
}

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Maybe<T>::doOnNext(const OnNextMethod &method)
{
	return Maybe<T>(std::make_shared<processors::Tap<T>>(*this, method, nullptr, nullptr));
}

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Maybe<T>::doOnTerminate(const OnCompleteMethod &method)
{
	return Maybe<T>(std::make_shared<processors::Tap<T>>(
		*this, nullptr, [method](const std::exception_ptr &) { method(); }, method));
}

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Maybe<T>::tap(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod)
{
	return Maybe<T>(std::make_shared<processors::Tap<T>>(*this, onNextMethod, onErrorMethod, onCompleteMethod));
}

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Maybe<T>::observeOn(recpp::async::Scheduler &scheduler)
{
	return Maybe<T>(std::make_shared<processors::ObserveOn<T>>(*this, scheduler));
}

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Maybe<T>::subscribeOn(recpp::async::Scheduler &scheduler)
{
	return Maybe<T>(std::make_shared<processors::SubscribeOn<T>>(*this, scheduler));
}

template <typename T>
template <typename Rep, typename Period>
recpp::rx::Maybe<T> recpp::rx::Maybe<T>::delay(recpp::async::Scheduler &scheduler, const std::chrono::duration<Rep, Period> &delay, bool delayError)
{
	return Maybe<T>(make_shared<recpp::processors::Delay<int, Rep, Period>>(*this, scheduler, delay, delayError));
}

template <typename T>
recpp::rx::Maybe<T>::Maybe(const std::shared_ptr<rscpp::Publisher<T>> &dd)
	: rscpp::Publisher<T>(dd)
{
}
