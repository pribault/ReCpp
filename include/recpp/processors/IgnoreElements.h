#pragma once

#include <rscpp/Processor.h>
#include <rscpp/Subscription.h>

namespace recpp::processors
{
	/**
	 * @class IgnoreElements IgnoreElements.h <recpp/processors/IgnoreElements.h>
	 * @brief {@link Processor} that will subscribe to a given {@link Publisher} and discard its elements.
	 *
	 * @param T The type of element signaled to the {@link Subscriber}.
	 * @param R The type of element signaled by the {@link Publisher}.
	 */
	template <typename T, typename R>
	class IgnoreElements : public rscpp::Processor<T, R>
	{
		class Impl : public rscpp::Processor<T, R>
		{
		public:
			explicit Impl(rscpp::Processor<T, R> &parent, const rscpp::Publisher<T> &source);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<R> &subscriber) override;

		private:
			rscpp::Processor<T, R> &m_parent;
			rscpp::Publisher<T>		m_source;
			rscpp::Subscriber<R>	m_subscriber;
			rscpp::Subscription		m_subscription;
			size_t					m_remaining = 0;
		};

	public:
		/**
		 * @brief Construct a new {@link IgnoreElements} instance.
		 *
		 * @param source The source {@link Publisher} the {@link IgnoreElements} {@link Processor} subscribes to.
		 */
		explicit IgnoreElements(const rscpp::Publisher<T> &source);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/IgnoreElements.inl>
