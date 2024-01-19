#pragma once

#include <rscpp/Subscriber.h>

namespace recpp::subscribers
{
	/**
	 * @class CompletableSubscriber CompletableSubscriber.h <recpp/subscribers/CompletableSubscriber.h>
	 *
	 * @brief Subscriber that can be used to create a new {@link recpp::Completable}.
	 * <p>
	 * This class does not inherit from {@link rscpp::Subscriber}.
	 */
	class CompletableSubscriber
	{
	public:
		/**
		 * @brief Construct a new {@link CompletableSubscriber} instance.
		 *
		 * @param subscriber The {@link rscpp::Subscriber} to forward signals to.
		 */
		CompletableSubscriber(const rscpp::Subscriber<int> &subscriber);

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
		rscpp::Subscriber<int> m_subscriber;
		bool				   m_ended = false;
	};
} // namespace recpp::subscribers
