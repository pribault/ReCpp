#pragma once

#include <rscpp/Subscriber.h>

namespace recpp::subscribers
{
	/**
	 * @class ObservableSubscriber ObservableSubscriber.h <recpp/subscribers/ObservableSubscriber.h>
	 *
	 * @brief Subscriber that can be used to create a new {@link rx::Observable}.
	 * <p>
	 * This class does not inherit from {@link rscpp::Subscriber}.
	 * @param T The type of element signaled.
	 */
	template <typename T>
	class ObservableSubscriber
	{
	public:
		/**
		 * @brief Construct a new {@link ObservableSubscriber} instance.
		 *
		 * @param subscriber The {@link rscpp::Subscriber} to forward signals to.
		 */
		ObservableSubscriber(const rscpp::Subscriber<T> &subscriber);

		/**
		 * @brief Emits the given value to the {@link rscpp::Subscriber} if it has not already been emitted.
		 *
		 * @param value The value to emit.
		 */
		void onNext(const T &value);

		/**
		 * @brief Emits the given error to the {@link rscpp::Subscriber} if it has not already been emitted.
		 *
		 * @param error The error to emit.
		 */
		void onError(const std::exception_ptr &error);

		/**
		 * @brief Emits a completion signal to the {@link rscpp::Subscriber} if it has not already been emitted.
		 */
		void onComplete();

	private:
		rscpp::Subscriber<T> m_subscriber;
		bool				 m_ended = false;
	};
} // namespace recpp::subscribers

#include <recpp/subscribers/inl/ObservableSubscriber.inl>
