#pragma once

#include <recpp/subscribers/SingleSubscriber.h>
#include <rscpp/Publisher.h>

#include <functional>

namespace recpp::async
{
	class Scheduler;
}

namespace recpp::subscribers
{
	template <typename T>
	class SingleSubscriber;
}

namespace recpp::rx
{
	class Completable;
	template <typename T>
	class Single;
	template <typename T>
	class Observable;

	template <typename T>
	class Single : public rscpp::Publisher<T>
	{
		template <typename R>
		friend class Single;
		friend class Completable;

	public:
		using OnSuccessMethod = std::function<void(const T & /* value */)>;
		using OnErrorMethod = std::function<void(const std::exception_ptr & /* error */)>;
		using OnCompleteMethod = std::function<void()>;

		/**
		 * @brief Construct a new {@link Single} instance that will call the given method when subscribed to.
		 * <p>
		 * The first parameter of the method will be a {@link recpp::subscriber::SingleSubscriber} that can be used to either complete or error the {@link
		 * Single}.
		 *
		 * @param method The method to call when subscribed to.
		 * @return The new {@link Single} instance.
		 */
		static Single<T> create(const std::function<void(recpp::subscribers::SingleSubscriber<T> &)> &method);

		/**
		 * @brief Construct a new {@link Single} instance that will call the given method when subscribed to.
		 * <p>
		 * The method must return a {@link Single} instance that will be subscribed to after returning.
		 *
		 * @return The new {@link Single} instance.
		 */
		static Single<T> defer(const std::function<Single<T>()> &function);

		/**
		 * @brief Construct a new {@link Single} instance that will emit the given error when subscribed to.
		 *
		 * @param error The error to emit.
		 * @return The new {@link Single} instance.
		 */
		static Single<T> error(const std::exception_ptr &error);

		/**
		 * @brief Construct a new {@link Single} instance that will emit the given value when subscribed to.
		 *
		 * @param value The value to emit.
		 * @return The new {@link Single} instance.
		 */
		static Single<T> just(const T &value);

		/**
		 * @brief Construct a new {@link Single} instance that will never emit a signal when subscribed to.
		 *
		 * @return The new {@link Single} instance.
		 */
		static Single<T> never();

		/**
		 * @brief Subscribe to this {@link Single} with the given methods.
		 *
		 * @param onSuccess The method to call when the {@link Single} emits a value.
		 * @param onError The method to call when the {@link Single} errors.
		 */
		void subscribe(const OnSuccessMethod &onSuccess = nullptr, const OnErrorMethod &onError = nullptr);

		/**
		 * @brief Apply the given method to the value emitted by this {@link Single}, and return a new {@link Single} containing the result.
		 *
		 * @tparam R The method return type.
		 * @param method The method to apply to the value.
		 * @return The new {@link Single} instance.
		 */
		template <typename R>
		Single<R> map(const std::function<R(const T & /* value */)> &method);

		/**
		 * @brief Apply the given method to the value emitted by this {@link Single}, and return a new {@link Single} containing the result.
		 * <p>
		 * The method must return a {@link Single} instance that will be subscribed to after returning.
		 *
		 * @tparam R The method return type.
		 * @param method The method to apply to the value.
		 * @return The new {@link Single} instance.
		 */
		template <typename R>
		Single<R> flatMap(const std::function<Single<R>(const T & /* value */)> &method);

		/**
		 * Convert this {@link Single} into a {@link Completable} by discarding any contained value.
		 *
		 * @return The new {@link Completable} instance.
		 */
		Completable ignoreElement();

		/**
		 * @brief Define a method that will be called on error.
		 *
		 * @param method The method to call on error.
		 * @return The new {@link Single} instance.
		 */
		Single<T> doOnError(const OnErrorMethod &method);

		/**
		 * @brief Define a method that will be called when the {@link Single} emits a value.
		 *
		 * @param method The method to call for each value.
		 * @return The new {@link Single} instance.
		 */
		Single<T> doOnSuccess(const OnSuccessMethod &method);

		/**
		 * @brief Define a method that will be called when the {@link Single} terminates, either by completing or erroring.
		 *
		 * @param method The method to call on termination.
		 * @return The new {@link Single} instance.
		 */
		Single<T> doOnTerminate(const OnCompleteMethod &method);

		/**
		 * @brief Define the methods that will be called when the {@link Single} emits a value, completes or errors.
		 *
		 * @param onNextMethod The method to call for each value.
		 * @param onErrorMethod The method to call on error.
		 * @param onCompleteMethod The method to call on completion.
		 * @return The new {@link Single} instance.
		 */
		Single<T> tap(const OnSuccessMethod &onSuccessMethod, const OnErrorMethod &onErrorMethod);

		/**
		 * @brief Forwards all emissions on the given {@link Scheduler}.
		 *
		 * @param scheduler The {@link Scheduler} to observe on.
		 * @return The new {@link Single} instance.
		 */
		Single<T> observeOn(async::Scheduler &scheduler);

		/**
		 * @brief Subscribe to this {@link Single} on the given {@link Scheduler}.
		 *
		 * @param scheduler The {@link Scheduler} to subscribe on.
		 * @return The new {@link Single} instance.
		 */
		Single<T> subscribeOn(async::Scheduler &scheduler);

	protected:
		Single(const std::shared_ptr<rscpp::Publisher<T>> &dd);
	};
} // namespace recpp::rx

#include <recpp/rx/inl/Single.inl>
