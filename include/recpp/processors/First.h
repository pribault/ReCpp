#pragma once

#include <rscpp/Processor.h>

#include <atomic>
#include <functional>
#include <mutex>
#include <optional>

namespace recpp::processors
{
	/**
	 * @class First First.h <recpp/processors/First.h>
	 * @brief {@link rscpp::Processor} that will subscribe to a given {@link rscpp::Publisher}, retrieve the first element and emit it. If the
	 * {@link rscpp::Publisher} is empty a default constructed value will be emited.
	 *
	 * @param T The type of element signaled to the {@link rscpp::Subscriber}.
	 */
	template <typename T>
	class First : public rscpp::Processor<T, T>
	{
		class Impl : public rscpp::Processor<T, T>
		{
		public:
			explicit Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<T> &subscriber) override;

		private:
			rscpp::Processor<T, T>		 &m_parent;
			rscpp::Publisher<T>			  m_publisher;
			rscpp::Subscriber<T>		  m_subscriber;
			rscpp::Subscription			  m_subscription;
			std::atomic<std::optional<T>> m_result;
		};

	public:
		/**
		 * @brief Construct a new {@link First} instance.
		 *
		 * @param publisher The source {@link rscpp::Publisher} the {@link First} {@link rscpp::Processor} subscribes to.
		 */
		explicit First(const rscpp::Publisher<T> &publisher);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/First.inl>
