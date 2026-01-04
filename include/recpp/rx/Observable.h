#pragma once

#include <recpp/subscribers/ObservableSubscriber.h>
#include <rscpp/Publisher.h>

#include <chrono>
#include <functional>
#include <optional>

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
	class Observable;
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
		friend class Completable;
		template <typename R>
		friend class Maybe;
		template <typename R>
		friend class Observable;
		template <typename R>
		friend class Single;

	public:
		/**
		 * @brief The type of the method to call for each emitted value.
		 */
		using OnNextMethod = std::function<void(const T & /* value */)>;

		/**
		 * @brief The type of the method to call on error.
		 */
		using OnErrorMethod = std::function<void(const std::exception_ptr & /* error */)>;

		/**
		 * @brief The type of the method to call on completion.
		 */
		using OnCompleteMethod = std::function<void()>;

		/**
		 * @brief Construct a new {@link Observable} instance that will call the given method when subscribed to.
		 * <p>
		 * The first parameter of the method will be a {@link subscribers::ObservableSubscriber} that can be used to emit values, errors or completion.
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
		 * @param method The method to call when subscribed to.
		 * @return The new {@link Observable} instance.
		 */
		static Observable<T> defer(const std::function<Observable<T>()> &method);

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
		 * @brief Construct a new {@link Observable} instance that will emit the given error when subscribed to.
		 *
		 * @param error The error to emit.
		 * @return The new {@link Observable} instance.
		 */
		template <typename E>
		static Observable<T> error(E error);

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

		/**
		 * @brief Construct a new {@link Observable} instance that will emit values contained inside the given range.
		 *
		 * @tparam I The range iterator type.
		 * @param first The first iterator.
		 * @param last The last iterator.
		 * @return The new {@link Observable} instance.
		 */
		template <class I>
		static Observable<T> range(I first, I last);

		/**
		 * @brief Construct a new {@link Observable} instance that will emit values contained inside the given range.
		 *
		 * @tparam R The range type.
		 * @param range The range.
		 * @return The new {@link Observable} instance.
		 */
		template <class R>
		static Observable<T> range(R &&range);

		/**
		 * @brief Construct a new {@link Observable} instance resulting of the merge of the given {@link Observable} instances.
		 *
		 * @param observableSource The source providing the {@link Observable} instances to merge.
		 * @param scheduler The optional scheduler to merge values on.
		 * @return The new {@link Observable} instance.
		 */
		static Observable<T> merge(Observable<Observable<T>>											&observableSource,
								   const std::optional<std::reference_wrapper<recpp::async::Scheduler>> &scheduler = std::nullopt);

		/**
		 * @brief Construct a new {@link Observable} instance that will emit values at a specified interval.
		 *
		 * @param period The period at which to emit the values.
		 * @param scheduler The scheduler to emit values on.
		 * @return The new {@link Observable} instance.
		 */
		static Observable<T> interval(const recpp::async::Scheduler::Duration &period, recpp::async::Scheduler &scheduler);

		/**
		 * @brief Construct a new {@link Observable} instance that will emit values at a specified interval.
		 *
		 * @param start The time point at which to start emitting values.
		 * @param period The period at which to emit the values.
		 * @param scheduler The scheduler to emit values on.
		 * @return The new {@link Observable} instance.
		 */
		static Observable<T> interval(const recpp::async::Scheduler::TimePoint &start, const recpp::async::Scheduler::Duration &period,
									  recpp::async::Scheduler &scheduler);

		/**
		 * @brief Subscribe to this {@link Observable} with the given methods.
		 *
		 * @param onNext The method to call when the {@link Observable} emits a value.
		 * @param onError The method to call when the {@link Observable} errors.
		 * @param onComplete The method to call when the {@link Observable} completes.
		 */
		void subscribe(const OnNextMethod &onNext = nullptr, const OnErrorMethod &onError = nullptr, const OnCompleteMethod &onComplete = nullptr);

		/**
		 * @brief Filter emitted values with the given method.
		 *
		 * @param method The method to apply to the value.
		 * @return The new {@link Observable} instance.
		 */
		Observable<T> filter(const std::function<bool(const T & /* value */)> &method);

		/**
		 * @brief Emits only the first count items emitted.
		 *
		 * @param count The maximum number of items to emit.
		 * @return The new {@link Observable} instance.
		 */
		Observable<T> take(std::size_t count);

		/**
		 * @brief Emits items as long as each item satisfies the given predicate.
		 *
		 * @param predicate The predicate the items must satisfy.
		 * @return The new {@link Observable} instance.
		 */
		Observable<T> takeWhile(const std::function<bool(const T & /* value */)> &predicate);

		/**
		 * @brief Emits items until one item satisfies the given predicate.
		 *
		 * @param stopPredicate The stop predicate.
		 * @return The new {@link Observable} instance.
		 */
		Observable<T> takeUntil(const std::function<bool(const T & /* value */)> &stopPredicate);

		/**
		 * Convert this {@link Observable} into a {@link Completable} by discarding all contained value.
		 *
		 * @return The new {@link Completable} instance.
		 */
		Completable ignoreElements();

		/**
		 * @brief Apply a given method to the values emitted by this {@link Observable}.
		 *
		 * @tparam R The method return type.
		 * @param method The method to apply to the values.
		 * @return The new {@link Observable} instance.
		 */
		template <typename R>
		Observable<R> map(const std::function<R(const T & /* value */)> &method);

		/**
		 * @brief Apply the given method to the values emitted by this {@link Observable}.
		 * <p>
		 * The method must return a {@link Observable} instance that will be subscribed to after returning.
		 *
		 * @tparam R The method return type.
		 * @param method The method to apply to the value.
		 * @return The new {@link Observable} instance.
		 */
		template <typename R>
		Observable<R> flatMap(const std::function<Observable<R>(const T & /* value */)> &method);

		/**
		 * @brief Define a method that will be called when the {@link Observable} completes.
		 *
		 * @param method The method to call on completion.
		 * @return The new {@link Observable} instance.
		 */
		Observable<T> doOnComplete(const OnCompleteMethod &method);

		/**
		 * @brief Define a method that will be called on error.
		 *
		 * @param method The method to call on error.
		 * @return The new {@link Observable} instance.
		 */
		Observable<T> doOnError(const OnErrorMethod &method);

		/**
		 * @brief Define a method that will be called when the {@link Observable} emits a value.
		 *
		 * @param method The method to call for each value.
		 * @return The new {@link Observable} instance.
		 */
		Observable<T> doOnNext(const OnNextMethod &method);

		/**
		 * @brief Define a method that will be called when the {@link Observable} terminates, either by completing or erroring.
		 *
		 * @param method The method to call on termination.
		 * @return The new {@link Observable} instance.
		 */
		Observable<T> doOnTerminate(const OnCompleteMethod &method);

		/**
		 * @brief Define the methods that will be called when the {@link Observable} emits a value, completes or errors.
		 *
		 * @param onNextMethod The method to call for each value.
		 * @param onErrorMethod The method to call on error.
		 * @param onCompleteMethod The method to call on completion.
		 * @return The new {@link Observable} instance.
		 */
		Observable<T> tap(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod);

		/**
		 * @brief Forwards all emissions on the given {@link async::Scheduler}.
		 *
		 * @param scheduler The {@link async::Scheduler} to observe on.
		 * @return The new {@link Observable} instance.
		 */
		Observable<T> observeOn(async::Scheduler &scheduler);

		/**
		 * @brief Subscribe to this {@link Observable} on the given {@link async::Scheduler}.
		 *
		 * @param scheduler The {@link async::Scheduler} to subscribe on.
		 * @return The new {@link Observable} instance.
		 */
		Observable<T> subscribeOn(async::Scheduler &scheduler);

		/**
		 * @brief Delay this {@link Observable} emissions.
		 *
		 * @tparam Rep The duration arithmetic type representing the number of ticks of the delay duration.
		 * @tparam Period The duration period type representing the tick period (i.e. the number of second's fractions per tick) of the delay duration.
		 * @param scheduler The {@link async::Scheduler} to delay emissions on.
		 * @param delay The minimum duration before emissions will be received.
		 * @param delayError Boolean defining if error emissions should also be delayed.
		 * @return The new {@link Observable} instance.
		 */
		template <typename Rep, typename Period>
		Observable<T> delay(async::Scheduler &scheduler, const std::chrono::duration<Rep, Period> &delay, bool delayError);

		/**
		 * @brief Emit the given value in case this {@link Observable} is empty.
		 *
		 * @param defaultValue The default value to emit.
		 * @return The new {@link Observable} instance.
		 */
		Observable<T> defaultIfEmpty(const T &defaultValue);

		/**
		 * @brief Checks if the given predicate pred returns true for all emited values.
		 *
		 * @tparam R The predicate type.
		 * @param predicate The predicate to apply to the values.
		 * @return The new {@link Observable} instance.
		 */
		template <typename R>
		Single<bool> allOf(R predicate);

		/**
		 * @brief Checks if the given predicate pred returns true for any emited values.
		 *
		 * @tparam R The predicate type.
		 * @param predicate The predicate to apply to the values.
		 * @return The new {@link Observable} instance.
		 */
		template <typename R>
		Single<bool> anyOf(R predicate);

		/**
		 * @brief Checks if the given predicate pred returns false for all emited values.
		 *
		 * @tparam R The predicate type.
		 * @param predicate The predicate to apply to the values.
		 * @return The new {@link Observable} instance.
		 */
		template <typename R>
		Single<bool> noneOf(R predicate);

		/**
		 * @brief Accumulate this {@link Observable} values and emit the result.
		 *
		 * @param init The initial value.
		 * @return The new {@link Observable} instance.
		 */
		Single<T> reduce(T init = 0);

		/**
		 * @brief Accumulate this {@link Observable} values and emit the result.
		 *
		 * @tparam R The operation type.
		 * @param operation The operation to use to add two values.
		 * @param init The initial value.
		 * @return The new {@link Observable} instance.
		 */
		template <typename R>
		Single<T> reduce(R operation, T init = 0);

		/**
		 * @brief Retrieve the maximum value of this stream and return the result as a {@link Single}.
		 *
		 * @return The new {@link Single} instance.
		 */
		Single<T> max();

		/**
		 * @brief Retrieve the maximum value of this stream and return the result as a {@link Single}.
		 *
		 * @tparam R The comparator type.
		 * @param comparator The comparator to use to compare two values;
		 * @return The new {@link Single} instance.
		 */
		template <typename R>
		Single<T> max(R comparator);

		/**
		 * @brief Retrieve the minimum value of this stream and return the result as a {@link Single}.
		 *
		 * @return The new {@link Single} instance.
		 */
		Single<T> min();

		/**
		 * @brief Retrieve the minimum value of this stream and return the result as a {@link Single}.
		 *
		 * @tparam R The comparator type.
		 * @param comparator The comparator to use to compare two values;
		 * @return The new {@link Single} instance.
		 */
		template <typename R>
		Single<T> min(R comparator);

		/**
		 * @brief Count the number of values of this stream and return the result as a {@link Single}.
		 *
		 * @return The new {@link Single} instance.
		 */
		Single<std::size_t> count();

		/**
		 * @brief Retrieve the element at the specified index as a {@link Maybe}.
		 *
		 * @param index The element index.
		 * @return The new {@link Maybe} instance.
		 */
		Maybe<T> elementAt(std::size_t index);

		/**
		 * @brief Retrieve the first element as a {@link Maybe}.
		 *
		 * @return The new {@link Maybe} instance.
		 */
		Maybe<T> first();

		/**
		 * @brief Retrieve the last element as a {@link Maybe}.
		 *
		 * @return The new {@link Maybe} instance.
		 */
		Maybe<T> last();

	protected:
		using rscpp::Publisher<T>::subscribe;

		/**
		 * @brief Construct a new {@link Observable} instance with the given private implementation.
		 *
		 * @param dd The private implementation.
		 */
		Observable(const std::shared_ptr<rscpp::Publisher<T>> &dd);
	};
} // namespace recpp::rx

#include <recpp/rx/inl/Observable.inl>
