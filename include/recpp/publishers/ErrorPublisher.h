#pragma once

#include <rscpp/Publisher.h>

#include <exception>

namespace recpp::publishers
{
	/**
	 * @class ErrorPublisher ErrorPublisher.h <recpp/publishers/ErrorPublisher.h>
	 * @brief {@link Publisher} that will emit no values nor complete, but simply emit an error.
	 *
	 * @tparam T The type of element signaled by the {@link Publisher}.
	 */
	template <typename T>
	class ErrorPublisher : public rscpp::Publisher<T>
	{
	public:
		/**
		 * @brief Construct a new {@link ErrorPublisher} instance.
		 *
		 * @param error The error to emit.
		 */
		ErrorPublisher(const std::exception_ptr &error);

		/**
		 * @brief Request {@link Publisher} to start streaming data.
		 * <p>
		 * This is a "factory method" and can be called multiple times, each time starting a new {@link Subscription}.
		 * <p>
		 * Each {@link Subscription} will work for only a single {@link Subscriber}.
		 * <p>
		 * A {@link Subscriber} should only subscribe once to a single {@link Publisher}.
		 * <p>
		 * If the {@link Publisher} rejects the subscription attempt or otherwise fails it will
		 * signal the error via {@link Subscriber::onError(const std::exception_ptr &)}.
		 *
		 * @param subscriber The {@link Subscriber} that will consume signals from this {@link Publisher}
		 */
		void subscribe(rscpp::Subscriber<T> &subscriber) override;

	private:
		std::exception_ptr m_error;
	};
} // namespace recpp::publishers

#include <recpp/publishers/inl/ErrorPublisher.inl>
