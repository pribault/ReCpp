#pragma once

#include <recpp/subscribers/ObservableSubscriber.h>
#include <rscpp/Publisher.h>

#include <functional>

namespace recpp
{
	template <typename T>
	class ObservableSubscriber;

	template <typename T>
	class Observable : public rscpp::Publisher<T>
	{
		template <typename R>
		friend class Observable;

	public:
		using OnNextMethod = std::function<void(const T & /* value */)>;
		using OnErrorMethod = std::function<void(const std::exception_ptr & /* error */)>;
		using OnCompleteMethod = std::function<void()>;

		static Observable<T> create(const std::function<void(ObservableSubscriber<T> &)> &method);

		static Observable<T> defer(const std::function<Observable<T>()> &function);

		static Observable<T> empty();

		static Observable<T> error(const std::exception_ptr &error);

		static Observable<T> just(const T &value);

		static Observable<T> never();

		template <class I>
		static Observable<T> range(I first, I last);

		template <class R>
		static Observable<T> range(R &&range);

		void subscribe(const OnNextMethod &onNext = nullptr, const OnErrorMethod &onError = nullptr, const OnCompleteMethod &onComplete = nullptr);

		Observable<T> filter(const std::function<bool(const T & /* value */)> &method);

		template <typename R>
		Observable<R> map(const std::function<R(const T & /* value */)> &method);

		template <typename R>
		Observable<R> flatMap(const std::function<Observable<R>(const T & /* value */)> &method);

		Observable<T> doOnComplete(const OnCompleteMethod &method);

		Observable<T> doOnError(const OnErrorMethod &method);

		Observable<T> doOnNext(const OnNextMethod &method);

		Observable<T> doOnTerminate(const OnCompleteMethod &method);

		Observable<T> tap(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod);

		Observable<T> observeOn(Scheduler &scheduler);

		Observable<T> subscribeOn(Scheduler &scheduler);

	protected:
		Observable(const std::shared_ptr<Publisher<T>> &dd);
	};
} // namespace recpp

#include <recpp/rx/inl/Observable.inl>
