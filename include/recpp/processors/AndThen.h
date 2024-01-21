#pragma once

#include <rscpp/Processor.h>
#include <rscpp/Subscription.h>

namespace recpp::processors
{
	/**
	 * @class AndThen AndThen.h <recpp/processors/AndThen.h>
	 * @brief {@link rscpp::Processor} that will subscribe to a given {@link rscpp::Publisher} and request items until completion AND THEN subscribe to a second
	 * {@link rscpp::Publisher} and forward its emissions.
	 *
	 * @param T The type of element signaled to the {@link rscpp::Subscriber}.
	 * @param R The type of element signaled by the {@link rscpp::Publisher}.
	 */
	template <typename T, typename R>
	class AndThen : public rscpp::Processor<T, R>
	{
		class Impl : public rscpp::Processor<T, R>
		{
		public:
			explicit Impl(rscpp::Processor<T, R> &parent, const rscpp::Publisher<T> &source, const rscpp::Publisher<R> &dest);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<R> &subscriber) override;

		private:
			rscpp::Processor<T, R> &m_parent;
			rscpp::Publisher<T>		m_source;
			rscpp::Publisher<R>		m_dest;
			rscpp::Subscriber<R>	m_subscriber;
			rscpp::Subscription		m_subscription;
			size_t					m_remaining = 0;
		};

	public:
		/**
		 * @brief Construct a new {@link AndThen} instance.
		 *
		 * @param source The source {@link rscpp::Publisher} the {@link AndThen} {@link rscpp::Processor} subscribes to and request items until completion.
		 * @param dest The destination {@link rscpp::Publisher} the {@link AndThen} {@link rscpp::Processor} will subscribe to after the source
		 * {@link rscpp::Publisher} completion.
		 */
		explicit AndThen(const rscpp::Publisher<T> &source, const rscpp::Publisher<R> &dest);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/AndThen.inl>
