#pragma once

#include <recpp/async/Scheduler.h>
#include <recpp/processors/Delay.h>
#include <recpp/processors/Filter.h>
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
#include <recpp/publishers/MergePublisher.h>
#include <recpp/publishers/NeverPublisher.h>
#include <recpp/publishers/RangePublisher.h>
#include <recpp/rx/Completable.h>
#include <recpp/subscribers/DefaultSubscriber.h>
#include <recpp/subscribers/ObservableSubscriber.h>

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::create(const std::function<void(recpp::subscribers::ObservableSubscriber<T> &)> &method)
{
	return Observable<T>(std::make_shared<recpp::publishers::CreatePublisher<T, recpp::subscribers::ObservableSubscriber<T>>>(method));
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::defer(const std::function<Observable<T>()> &function)
{
	return Observable<T>(std::make_shared<recpp::publishers::DeferPublisher<T, Observable<T>>>(function));
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::empty()
{
	return Observable<T>(std::make_shared<recpp::publishers::EmptyPublisher<T>>());
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::error(const std::exception_ptr &error)
{
	return Observable<T>(std::make_shared<recpp::publishers::ErrorPublisher<T>>(error));
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::just(const T &value)
{
	return Observable<T>(std::make_shared<recpp::publishers::JustPublisher<T>>(value));
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::never()
{
	return Observable<T>(std::make_shared<recpp::publishers::NeverPublisher<T>>());
}

template <typename T>
template <class I>
recpp::rx::Observable<T> recpp::rx::Observable<T>::range(I first, I last)
{
	return Observable<T>(std::make_shared<recpp::publishers::RangePublisher<T, I>>(first, last));
}

template <typename T>
template <class R>
recpp::rx::Observable<T> recpp::rx::Observable<T>::range(R &&range)
{
	return Observable<T>::range(std::begin(range), std::end(range));
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::merge(recpp::rx::Observable<recpp::rx::Observable<T>> &observableSource)
{
	return Observable<T>(std::make_shared<recpp::publishers::MergePublisher<T, Observable<T>>>(observableSource));
}

template <typename T>
void recpp::rx::Observable<T>::subscribe(const OnNextMethod &onNext, const OnErrorMethod &onError, const OnCompleteMethod &onComplete)
{
	auto subscriber = recpp::subscribers::DefaultSubscriber<T>(onNext, onError, onComplete);
	rscpp::Publisher<T>::subscribe(subscriber);
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::filter(const std::function<bool(const T & /* value */)> &method)
{
	return Observable<T>(std::make_shared<processors::Filter<T>>(*this, method));
}

template <typename T>
recpp::rx::Completable recpp::rx::Observable<T>::ignoreElements()
{
	return Completable(std::make_shared<processors::IgnoreElements<T, int>>(*this));
}

template <typename T>
template <typename R>
recpp::rx::Observable<R> recpp::rx::Observable<T>::map(const std::function<R(const T & /* value */)> &method)
{
	return Observable<R>(std::make_shared<processors::Map<T, R>>(*this, method));
}

template <typename T>
template <typename R>
recpp::rx::Observable<R> recpp::rx::Observable<T>::flatMap(const std::function<Observable<R>(const T & /* value */)> &method)
{
	return Observable<R>(std::make_shared<processors::FlatMap<T, R>>(*this, method));
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::doOnComplete(const OnCompleteMethod &method)
{
	return Observable<T>(std::make_shared<processors::Tap<T>>(*this, nullptr, nullptr, method));
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::doOnError(const OnErrorMethod &method)
{
	return Observable<T>(std::make_shared<processors::Tap<T>>(*this, nullptr, method, nullptr));
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::doOnNext(const OnNextMethod &method)
{
	return Observable<T>(std::make_shared<processors::Tap<T>>(*this, method, nullptr, nullptr));
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::doOnTerminate(const OnCompleteMethod &method)
{
	return Observable<T>(std::make_shared<processors::Tap<T>>(
		*this, nullptr,
		[method](const std::exception_ptr &)
		{
			method();
		},
		method));
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::tap(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod,
													   const OnCompleteMethod &onCompleteMethod)
{
	return Observable<T>(std::make_shared<processors::Tap<T>>(*this, onNextMethod, onErrorMethod, onCompleteMethod));
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::observeOn(recpp::async::Scheduler &scheduler)
{
	return Observable<T>(std::make_shared<processors::ObserveOn<T>>(*this, scheduler));
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Observable<T>::subscribeOn(recpp::async::Scheduler &scheduler)
{
	return Observable<T>(std::make_shared<processors::SubscribeOn<T>>(*this, scheduler));
}

template <typename T>
template <typename Rep, typename Period>
recpp::rx::Observable<T> recpp::rx::Observable<T>::delay(recpp::async::Scheduler &scheduler, const std::chrono::duration<Rep, Period> &delay, bool delayError)
{
	return Observable<T>(std::make_shared<processors::Delay<int, Rep, Period>>(*this, scheduler, delay, delayError));
}

template <typename T>
recpp::rx::Observable<T>::Observable(const std::shared_ptr<rscpp::Publisher<T>> &dd)
	: rscpp::Publisher<T>(dd)
{
}
