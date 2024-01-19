#pragma once

#include <recpp/subscribers/ObservableSubscriber.h>
#include <rscpp/Publisher.h>

#include <functional>

namespace recpp::async
{
	class Scheduler;
}

namespace recpp::subscribers
{
	template <typename T>
	class ObservableSubscriber;
}

namespace recpp::rx
{
	class Completable;
	template <typename T>
	class Maybe;
	template <typename T>
	class Single;

	/**
	 * @class Observable Observable.h <recpp/rx/Observable.h>
	 *
	 * @brief An Observable is a Publisher that emits a stream of values.
	 * @param T The type of element signaled.
	 */
	template <typename T>
	class Observable : public rscpp::Publisher<T>
	{
		template <typename R>
		friend class Observable;
		friend class Completable;

	public:
		using OnNextMethod = std::function<void(const T & /* value */)>;
		using OnErrorMethod = std::function<void(const std::exception_ptr & /* error */)>;
		using OnCompleteMethod = std::function<void()>;

		/**
		 * @brief Construct a new {@link Observable} instance that will call the given method when subscribed to.
		 * <p>
		 * The first parameter of the method will be a {@link recpp::subscribers::ObservableSubscriber} that can be used to emit values, errors or completion.
		 *
		 * @param method The method to call when subscribed to.
		 * @return The new {@link Observable} instance.
		 */
		static Observable<T> create(const std::function<void(recpp::subscribers::ObservableSubscriber<T> &)> &method);

		/**
		 * @brief Construct a new {@link Observable} instance that will call the given method when subscribed to.
		 * <p>
		 * The method must return a {@link Observable} instance that will be subscribed to after returning.
		 *
		 * @return The new {@link Observable} instance.
		 */
		static Observable<T> defer(const std::function<Observable<T>()> &function);

		/**
		 * @brief Construct a new {@link Observable} instance that will not emit any value when subscribed to.
		 *
		 * @return The new {@link Observable} instance.
		 */
		static Observable<T> empty();

		/**
		 * @brief Construct a new {@link Observable} instance that will emit the given error when subscribed to.
		 *
		 * @param error The error to emit.
		 * @return The new {@link Observable} instance.
		 */
		static Observable<T> error(const std::exception_ptr &error);

		/**
		 * @brief Construct a new {@link Observable} instance that will emit the given value when subscribed to.
		 *
		 * @param value The value to emit.
		 * @return The new {@link Observable} instance.
		 */
		static Observable<T> just(const T &value);

		/**
		 * @brief Construct a new {@link Observable} instance that will never emit a signal when subscribed to.
		 *
		 * @return The new {@link Observable} instance.
		 */
		static Observable<T> never();

		template <class I>
		static Observable<T> range(I first, I last);

		template <class R>
		static Observable<T> range(R &&range);

		void subscribe(const OnNextMethod &onNext = nullptr, const OnErrorMethod &onError = nullptr, const OnCompleteMethod &onComplete = nullptr);

		Observable<T> filter(const std::function<bool(const T & /* value */)> &method);

		Completable ignoreElements();

		template <typename R>
		Observable<R> map(const std::function<R(const T & /* value */)> &method);

		template <typename R>
		Observable<R> flatMap(const std::function<Observable<R>(const T & /* value */)> &method);

		Observable<T> doOnComplete(const OnCompleteMethod &method);

		Observable<T> doOnError(const OnErrorMethod &method);

		Observable<T> doOnNext(const OnNextMethod &method);

		Observable<T> doOnTerminate(const OnCompleteMethod &method);

		Observable<T> tap(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod);

		Observable<T> observeOn(async::Scheduler &scheduler);

		Observable<T> subscribeOn(async::Scheduler &scheduler);

	protected:
		Observable(const std::shared_ptr<rscpp::Publisher<T>> &dd);
	};
} // namespace recpp::rx

#include <recpp/rx/inl/Observable.inl>
