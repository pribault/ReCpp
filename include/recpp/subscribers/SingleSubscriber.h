#pragma once

#include <rscpp/Subscriber.h>

namespace recpp::subscribers
{
	/**
	 * @class SingleSubscriber SingleSubscriber.h <recpp/subscribers/SingleSubscriber.h>
	 *
	 * @brief Subscriber that can be used to create a new {@link rx::Single}.
	 * <p>
	 * This class does not inherit from {@link rscpp::Subscriber}.
	 * @param T The type of element signaled.
	 */
	template <typename T>
	class SingleSubscriber
	{
	public:
		/**
		 * @brief Construct a new {@link SingleSubscriber} instance.
		 *
		 * @param subscriber The {@link rscpp::Subscriber} to forward signals to.
		 */
		SingleSubscriber(const rscpp::Subscriber<T> &subscriber);

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

	private:
		rscpp::Subscriber<T> m_subscriber;
		bool				 m_ended = false;
	};
} // namespace recpp::subscribers

#include <recpp/subscribers/inl/SingleSubscriber.inl>
