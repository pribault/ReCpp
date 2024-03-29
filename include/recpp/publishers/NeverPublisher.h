#pragma once

#include <rscpp/Publisher.h>

namespace recpp::publishers
{
	/**
	 * @class NeverPublisher NeverPublisher.h <recpp/publishers/NeverPublisher.h>
	 * @brief {@link rscpp::Publisher} that will never emit values or errors. Completion will only happen on cancel.
	 *
	 * @tparam T The type of element signaled by the {@link rscpp::Publisher}.
	 */
	template <typename T>
	class NeverPublisher : public rscpp::Publisher<T>
	{
	public:
		/**
		 * @brief Construct a new {@link NeverPublisher} instance.
		 */
		NeverPublisher();

		/**
		 * @brief Request {@link rscpp::Publisher} to start streaming data.
		 * <p>
		 * This is a "factory method" and can be called multiple times, each time starting a new {@link rscpp::Subscription}.
		 * <p>
		 * Each {@link rscpp::Subscription} will work for only a single {@link rscpp::Subscriber}.
		 * <p>
		 * A {@link rscpp::Subscriber} should only subscribe once to a single {@link rscpp::Publisher}.
		 * <p>
		 * If the {@link rscpp::Publisher} rejects the subscription attempt or otherwise fails it will
		 * signal the error via {@link rscpp::Subscriber::onError(const std::exception_ptr &)}.
		 *
		 * @param subscriber The {@link rscpp::Subscriber} that will consume signals from this {@link rscpp::Publisher}
		 */
		void subscribe(rscpp::Subscriber<T> &subscriber) override;
	};
} // namespace recpp::publishers

#include <recpp/publishers/inl/NeverPublisher.inl>
