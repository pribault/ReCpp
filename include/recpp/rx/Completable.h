#pragma once

#include <recpp/subscribers/CompletableSubscriber.h>
#include <rscpp/Publisher.h>

#include <chrono>
#include <functional>

namespace recpp::async
{
	class Scheduler;
}

namespace recpp::subscriber
{
	class CompletableSubscriber;
}

namespace recpp::rx
{
	template <typename T>
	class Maybe;
	template <typename T>
	class Observable;
	template <typename T>
	class Single;

	/**
	 * @class Completable Completable.h <recpp/rx/Completable.h>
	 *
	 * @brief A Completable is a Publisher that emits either a completion or error signal.
	 */
	class Completable : public rscpp::Publisher<int>
	{
		template <typename T>
		friend class Maybe;
		template <typename T>
		friend class Observable;
		template <typename T>
		friend class Single;

	public:
		/**
		 * @brief The type of the method to call on error.
		 */
		using OnErrorMethod = std::function<void(const std::exception_ptr & /* error */)>;

		/**
		 * @brief The type of the method to call on completion.
		 */
		using OnCompleteMethod = std::function<void()>;

		/**
		 * @brief Construct a new {@link Completable} instance that will emit a completion signal when subscribed to.
		 *
		 * @return The new {@link Completable} instance.
		 */
		static Completable complete();

		/**
		 * @brief Construct a new {@link Completable} instance that will call the given method when subscribed to.
		 * <p>
		 * The first parameter of the method will be a {@link subscribers::CompletableSubscriber} that can be used to either complete or error the
		 * {@link Completable}.
		 *
		 * @param method The method to call when subscribed to.
		 * @return The new {@link Completable} instance.
		 */
		static Completable create(const std::function<void(recpp::subscribers::CompletableSubscriber &)> &method);

		/**
		 * @brief Construct a new {@link Completable} instance that will call the given method when subscribed to.
		 * <p>
		 * The method must return a {@link Completable} instance that will be subscribed to after returning.
		 *
		 * @param method The method to call when subscribed to.
		 * @return The new {@link Completable} instance.
		 */
		static Completable defer(const std::function<Completable()> &method);

		/**
		 * @brief Construct a new {@link Completable} instance that will emit the given error when subscribed to.
		 *
		 * @param error The error to emit.
		 * @return The new {@link Completable} instance.
		 */
		static Completable error(const std::exception_ptr &error);

		/**
		 * @brief Construct a new {@link Completable} instance that will never emit a signal when subscribed to.
		 *
		 * @return The new {@link Completable} instance.
		 */
		static Completable never();

		/**
		 * @brief Construct a new {@link Completable} instance resulting of the merge of the given {@link Completable} instances.
		 *
		 * @tparam I The range iterator type.
		 * @param first The first iterator.
		 * @param last The last iterator.
		 * @return The new {@link Completable} instance.
		 */
		template <class I>
		static Completable merge(I first, I last);

		/**
		 * @brief Construct a new {@link Completable} instance resulting of the merge of the given {@link Completable} instances.
		 *
		 * @tparam R The range type.
		 * @param range The range.
		 * @return The new {@link Completable} instance.
		 */
		template <class R>
		static Completable merge(R &&range);

		/**
		 * @brief Subscribe to this {@link Completable} with the given methods.
		 *
		 * @param onComplete The method to call when the {@link Completable} completes.
		 * @param onError The method to call when the {@link Completable} errors.
		 */
		void subscribe(const OnCompleteMethod &onComplete = nullptr, const OnErrorMethod &onError = nullptr);

		/**
		 * @brief Define a method that will be called when the {@link Completable} completes.
		 *
		 * @param method The method to call on completion.
		 * @return The new {@link Completable} instance.
		 */
		Completable doOnComplete(const OnCompleteMethod &method);

		/**
		 * @brief Define a method that will be called on error.
		 *
		 * @param method The method to call on error.
		 * @return The new {@link Completable} instance.
		 */
		Completable doOnError(const OnErrorMethod &method);

		/**
		 * @brief Define a method that will be called when the {@link Completable} terminates, either by completing or erroring.
		 *
		 * @param method The method to call on termination.
		 * @return The new {@link Completable} instance.
		 */
		Completable doOnTerminate(const OnCompleteMethod &method);

		/**
		 * @brief Define the methods that will be called when the {@link Completable} completes or errors.
		 *
		 * @param onCompleteMethod The method to call on completion.
		 * @param onErrorMethod The method to call on error.
		 * @return The new {@link Completable} instance.
		 */
		Completable tap(const OnCompleteMethod &onCompleteMethod, const OnErrorMethod &onErrorMethod);

		/**
		 * @brief Forwards all emissions on the given {@link async::Scheduler}.
		 *
		 * @param scheduler The {@link async::Scheduler} to observe on.
		 * @return The new {@link Completable} instance.
		 */
		Completable observeOn(async::Scheduler &scheduler);

		/**
		 * @brief Subscribe to this {@link Completable} on the given {@link async::Scheduler}.
		 *
		 * @param scheduler The {@link async::Scheduler} to subscribe on.
		 * @return The new {@link Completable} instance.
		 */
		Completable subscribeOn(async::Scheduler &scheduler);

		/**
		 * @brief define a {@link Completable} that will be subscribed to after this {@link Completable} completes.
		 *
		 * @param completable The {@link Completable} to subscribe to after this {@link Completable} completes.
		 * @return The new {@link Completable} instance.
		 */
		Completable andThen(const Completable &completable);

		/**
		 * @brief define a {@link Maybe} that will be subscribed to after this {@link Completable} completes.
		 *
		 * @param maybe The {@link Maybe} to subscribe to after this {@link Completable} completes.
		 * @return The new {@link Maybe} instance.
		 */
		template <typename T>
		Maybe<T> andThen(const Maybe<T> &maybe);

		/**
		 * @brief define a {@link Observable} that will be subscribed to after this {@link Completable} completes.
		 *
		 * @param observable The {@link Observable} to subscribe to after this {@link Completable} completes.
		 * @return The new {@link Observable} instance.
		 */
		template <typename T>
		Observable<T> andThen(const Observable<T> &observable);

		/**
		 * @brief define a {@link Single} that will be subscribed to after this {@link Completable} completes.
		 *
		 * @param maybe The {@link Single} to subscribe to after this {@link Completable} completes.
		 * @return The new {@link Single} instance.
		 */
		template <typename T>
		Single<T> andThen(const Single<T> &maybe);

		/**
		 * @brief Delay this {@link Completable} emissions.
		 *
		 * @tparam Rep The duration arithmetic type representing the number of ticks of the delay duration.
		 * @tparam Period The duration period type representing the tick period (i.e. the number of second's fractions per tick) of the delay duration.
		 * @param scheduler The {@link async::Scheduler} to delay emissions on.
		 * @param delay The minimum duration before emissions will be received.
		 * @param delayError Boolean defining if error emissions should also be delayed.
		 * @return The new {@link Completable} instance.
		 */
		template <typename Rep, typename Period>
		Completable delay(async::Scheduler &scheduler, const std::chrono::duration<Rep, Period> &delay, bool delayError);

	protected:
		/**
		 * @brief Construct a new {@link Completable} instance with the given private implementation.
		 *
		 * @param dd The private implementation.
		 */
		Completable(const std::shared_ptr<rscpp::Publisher<int>> &dd);
	};
} // namespace recpp::rx

#include <recpp/rx/inl/Completable.inl>
