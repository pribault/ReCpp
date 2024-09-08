#pragma once

#include <rscpp/Processor.h>

#include <functional>
#include <mutex>

namespace recpp::processors
{
	/**
	 * @class Min Min.h <recpp/processors/Min.h>
	 * @brief {@link rscpp::Processor} that will subscribe to a given {@link rscpp::Publisher}, compare the emitted values and emit the lowest one.
	 *
	 * @param T The type of element signaled to the {@link rscpp::Subscriber}.
	 * @param R The type of the comparator to call on each value.
	 */
	template <typename T, typename R>
	class Min : public rscpp::Processor<T, T>
	{
		class Impl : public rscpp::Processor<T, T>
		{
		public:
			explicit Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, R comparator);

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
			R						m_comparator;
			std::optional<T>		m_min;
			std::mutex				m_mutex;
		};

	public:
		/**
		 * @brief Construct a new {@link Min} instance.
		 *
		 * @param publisher The source {@link rscpp::Publisher} the {@link Min} {@link rscpp::Processor} subscribes to.
		 * @param comparator The comparator to call on each value.
		 */
		explicit Min(const rscpp::Publisher<T> &publisher, R comparator);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/Min.inl>
