#pragma once

#include <rscpp/Publisher.h>

#include <functional>

namespace recpp::publishers
{
	/**
	 * @class DeferPublisher DeferPublisher.h <recpp/publishers/DeferPublisher.h>
	 * @brief {@link rscpp::Publisher} that will call a given method on subscription, and subscribe to its result.
	 *
	 * @tparam T The type of element signaled by the {@link rscpp::Publisher}.
	 * @tparam P The method {@link rscpp::Publisher} return type.
	 */
	template <typename T, typename P>
	class DeferPublisher : public rscpp::Publisher<T>
	{
	public:
		/**
		 * @brief Construct a new {@link DeferPublisher} instance.
		 *
		 * @param method The method to call on subscription.
		 */
		DeferPublisher(const std::function<P()> &method);

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

	private:
		std::function<P()> m_method;
	};
} // namespace recpp::publishers

#include <recpp/publishers/inl/DeferPublisher.inl>
