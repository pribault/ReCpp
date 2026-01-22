#pragma once

#include <rscpp/Processor.h>

#include <functional>

namespace recpp::processors
{
	/**
	 * @class DefaultIfEmpty DefaultIfEmpty.h <recpp/processors/DefaultIfEmpty.h>
	 * @brief {@link rscpp::Processor} that will subscribe to a given {@link rscpp::Publisher} and emit a given default value in case the
	 * {@link rscpp::Publisher} completes without emiting any value.
	 *
	 * @param T The type of element signaled to the {@link rscpp::Subscriber} and by the {@link rscpp::Publisher}.
	 */
	template <typename T>
	class DefaultIfEmpty : public rscpp::Processor<T, T>
	{
		class Impl : public rscpp::Processor<T, T>
		{
		public:
			explicit Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, const T &defaultValue);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<T> &subscriber) override;

		private:
			rscpp::Processor<T, T> &m_parent;
			rscpp::Publisher<T>		m_publisher;
			rscpp::Subscriber<T>	m_subscriber;
			rscpp::Subscription		m_subscription;
			T						m_defaultValue;
			bool					m_gotValues = false;
		};

	public:
		/**
		 * @brief Construct a new {@link DefaultIfEmpty} instance.
		 *
		 * @param publisher The source {@link rscpp::Publisher} the {@link DefaultIfEmpty} {@link rscpp::Processor} subscribes to.
		 * @param defaultValue The default value to emit if the stream is empty.
		 */
		explicit DefaultIfEmpty(const rscpp::Publisher<T> &publisher, const T &defaultValue);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/DefaultIfEmpty.inl>
