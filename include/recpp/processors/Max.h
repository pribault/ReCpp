#pragma once

#include <rscpp/Processor.h>

#include <functional>
#include <mutex>

namespace recpp::processors
{
	/**
	 * @class Max Max.h <recpp/processors/Max.h>
	 * @brief {@link rscpp::Processor} that will subscribe to a given {@link rscpp::Publisher}, compare the emitted values and emit the highest one.
	 *
	 * @param T The type of element signaled to the {@link rscpp::Subscriber}.
	 * @param R The type of the comparator to call on each value.
	 */
	template <typename T, typename R>
	class Max : public rscpp::Processor<T, T>
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
			std::optional<T>		m_max;
			std::mutex				m_mutex;
		};

	public:
		/**
		 * @brief Construct a new {@link Max} instance.
		 *
		 * @param publisher The source {@link rscpp::Publisher} the {@link Max} {@link rscpp::Processor} subscribes to.
		 * @param comparator The comparator to call on each value.
		 */
		explicit Max(const rscpp::Publisher<T> &publisher, R comparator);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/Max.inl>
