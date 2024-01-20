#pragma once

#include <recpp/subscribers/MaybeSubscriber.h>
#include <rscpp/Publisher.h>

#include <functional>

namespace recpp::async
{
	class Scheduler;
}

namespace recpp::subscribers
{
	template <typename T>
	class MaybeSubscriber;
}

namespace recpp::rx
{
	class Completable;
	template <typename T>
	class Observable;
	template <typename T>
	class Single;

	/**
	 * @class Maybe Maybe.h <recpp/rx/Maybe.h>
	 *
	 * @brief A Maybe is a Publisher that emits either a single value, completion or error signal.
	 * @param T The type of element signaled.
	 */
	template <typename T>
	class Maybe : public rscpp::Publisher<T>
	{
		template <typename R>
		friend class Maybe;
		friend class Completable;

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
		 * @brief Construct a new {@link Maybe} instance that will call the given method when subscribed to.
		 * <p>
		 * The first parameter of the method will be a {@link subscribers::MaybeSubscriber} that can be used to either complete or error the {@link
		 * Maybe}.
		 *
		 * @param method The method to call when subscribed to.
		 * @return The new {@link Maybe} instance.
		 */
		static Maybe<T> create(const std::function<void(recpp::subscribers::MaybeSubscriber<T> &)> &method);

		/**
		 * @brief Construct a new {@link Maybe} instance that will call the given method when subscribed to.
		 * <p>
		 * The method must return a {@link Maybe} instance that will be subscribed to after returning.
		 *
		 * @param method The method to call when subscribed to.
		 * @return The new {@link Maybe} instance.
		 */
		static Maybe<T> defer(const std::function<Maybe<T>()> &method);

		/**
		 * @brief Construct a new {@link Maybe} instance that will not emit any value when subscribed to.
		 *
		 * @return The new {@link Maybe} instance.
		 */
		static Maybe<T> empty();

		/**
		 * @brief Construct a new {@link Maybe} instance that will emit the given error when subscribed to.
		 *
		 * @param error The error to emit.
		 * @return The new {@link Maybe} instance.
		 */
		static Maybe<T> error(const std::exception_ptr &error);

		/**
		 * @brief Construct a new {@link Maybe} instance that will emit the given value when subscribed to.
		 *
		 * @param value The value to emit.
		 * @return The new {@link Maybe} instance.
		 */
		static Maybe<T> just(const T &value);

		/**
		 * @brief Construct a new {@link Maybe} instance that will never emit a signal when subscribed to.
		 *
		 * @return The new {@link Maybe} instance.
		 */
		static Maybe<T> never();

		/**
		 * @brief Subscribe to this {@link Maybe} with the given methods.
		 *
		 * @param onNext The method to call when the {@link Maybe} emits a value.
		 * @param onError The method to call when the {@link Maybe} errors.
		 * @param onComplete The method to call when the {@link Maybe} completes.
		 */
		void subscribe(const OnNextMethod &onNext = nullptr, const OnErrorMethod &onError = nullptr, const OnCompleteMethod &onComplete = nullptr);

		/**
		 * @brief Apply the given method to the value emitted by this {@link Maybe} if any, and return a new {@link Maybe} containing the result.
		 *
		 * @tparam R The method return type.
		 * @param method The method to apply to the value.
		 * @return The new {@link Maybe} instance.
		 */
		template <typename R>
		Maybe<R> map(const std::function<R(const T & /* value */)> &method);

		/**
		 * @brief Apply the given method to the value emitted by this {@link Maybe} if any, and return a new {@link Maybe} containing the result.
		 * <p>
		 * The method must return a {@link Maybe} instance that will be subscribed to after returning.
		 *
		 * @tparam R The method return type.
		 * @param method The method to apply to the value.
		 * @return The new {@link Maybe} instance.
		 */
		template <typename R>
		Maybe<R> flatMap(const std::function<Maybe<R>(const T & /* value */)> &method);

		/**
		 * Convert this {@link Maybe} into a {@link Completable} by discarding any contained value.
		 *
		 * @return The new {@link Completable} instance.
		 */
		Completable ignoreElement();

		/**
		 * @brief Define a method that will be called when the {@link Maybe} completes.
		 *
		 * @param method The method to call on completion.
		 * @return The new {@link Maybe} instance.
		 */
		Maybe<T> doOnComplete(const OnCompleteMethod &method);

		/**
		 * @brief Define a method that will be called on error.
		 *
		 * @param method The method to call on error.
		 * @return The new {@link Maybe} instance.
		 */
		Maybe<T> doOnError(const OnErrorMethod &method);

		/**
		 * @brief Define a method that will be called when the {@link Maybe} emits a value.
		 *
		 * @param method The method to call for each value.
		 * @return The new {@link Maybe} instance.
		 */
		Maybe<T> doOnNext(const OnNextMethod &method);

		/**
		 * @brief Define a method that will be called when the {@link Maybe} terminates, either by completing or erroring.
		 *
		 * @param method The method to call on termination.
		 * @return The new {@link Maybe} instance.
		 */
		Maybe<T> doOnTerminate(const OnCompleteMethod &method);

		/**
		 * @brief Define the methods that will be called when the {@link Maybe} emits a value, completes or errors.
		 *
		 * @param onNextMethod The method to call for each value.
		 * @param onErrorMethod The method to call on error.
		 * @param onCompleteMethod The method to call on completion.
		 * @return The new {@link Maybe} instance.
		 */
		Maybe<T> tap(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod);

		/**
		 * @brief Forwards all emissions on the given {@link async::Scheduler}.
		 *
		 * @param scheduler The {@link async::Scheduler} to observe on.
		 * @return The new {@link Maybe} instance.
		 */
		Maybe<T> observeOn(async::Scheduler &scheduler);

		/**
		 * @brief Subscribe to this {@link Maybe} on the given {@link async::Scheduler}.
		 *
		 * @param scheduler The {@link async::Scheduler} to subscribe on.
		 * @return The new {@link Maybe} instance.
		 */
		Maybe<T> subscribeOn(async::Scheduler &scheduler);

	protected:
		/**
		 * @brief Construct a new {@link Maybe} instance with the given private implementation.
		 * 
		 * @param dd The private implementation.
		 */
		Maybe(const std::shared_ptr<rscpp::Publisher<T>> &dd);
	};
} // namespace recpp::rx

#include <recpp/rx/inl/Maybe.inl>
