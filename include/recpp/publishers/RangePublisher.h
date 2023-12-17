#pragma once

#include <rscpp/Publisher.h>

namespace recpp::publishers
{
	/**
	 * @class RangePublisher RangePublisher.h <recpp/publishers/RangePublisher.h>
	 * @brief {@link Publisher} that will emit a range of values using given iterators.
	 *
	 * @tparam T The type of element signaled by the {@link Publisher}.
	 * @tparam I The iterators type.
	 */
	template <typename T, class I>
	class RangePublisher : public rscpp::Publisher<T>
	{
	public:
		/**
		 * @brief Construct a new {@link RangePublisher} instance.
		 *
		 * @param first The begin iterator.
		 * @param last The end iterator.
		 */
		RangePublisher(I first, I last);

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
		I m_first;
		I m_last;
	};
} // namespace recpp::publishers

#include <recpp/publishers/inl/RangePublisher.inl>
