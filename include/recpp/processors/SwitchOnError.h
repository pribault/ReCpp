#pragma once

#include <rscpp/Processor.h>

#include <recpp/subscriptions/FilterSubscription.h>

#include <functional>
#include <vector>

namespace recpp::processors
{
	/**
	 * @class SwitchOnError SwitchOnError.h <recpp/processors/SwitchOnError.h>
	 * @brief {@link rscpp::Processor} that will subscribe to a given {@link rscpp::Publisher} and switch to another {@link rscpp::Publisher} if the first one
	 * emits an error.
	 *
	 * @tparam T The type of element signaled to the {@link rscpp::Subscriber} and signaled from the {@link rscpp::Publisher}.
	 */
	template <typename T>
	class SwitchOnError : public rscpp::Processor<T, T>
	{
		class Impl : public rscpp::Processor<T, T>
		{
		public:
			explicit Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, const rscpp::Publisher<T> &fallbackPublisher);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<T> &subscriber) override;

		private:
			rscpp::Processor<T, T> &m_parent;
			rscpp::Publisher<T>		m_publisher;
			rscpp::Publisher<T>		m_fallbackPublisher;
			rscpp::Subscriber<T>	m_subscriber;
			bool					m_publisherErrored = false;
		};

	public:
		/**
		 * @brief Construct a new {@link SwitchOnError} instance.
		 *
		 * @param publisher The source {@link rscpp::Publisher} the {@link SwitchOnError} {@link rscpp::Processor} subscribes to.
		 * @param fallbackPublisher The second {@link rscpp::Publisher} the {@link SwitchOnError} {@link rscpp::Processor} subscribes to in case the first one
		 * emits an error.
		 */
		explicit SwitchOnError(const rscpp::Publisher<T> &publisher, const rscpp::Publisher<T> &fallbackPublisher);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/SwitchOnError.inl>
