#pragma once

#include <rscpp/Processor.h>

#include <atomic>
#include <cstddef>
#include <functional>

namespace recpp::processors
{
	/**
	 * @class Count Count.h <recpp/processors/Count.h>
	 * @brief {@link rscpp::Processor} that will subscribe to a given {@link rscpp::Publisher}, count the number of emitted values and emit the result.
	 *
	 * @param T The type of element signaled to the {@link rscpp::Subscriber}.
	 */
	template <typename T>
	class Count : public rscpp::Processor<T, std::size_t>
	{
		class Impl : public rscpp::Processor<T, std::size_t>
		{
		public:
			explicit Impl(rscpp::Processor<T, std::size_t> &parent, const rscpp::Publisher<T> &publisher);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<std::size_t> &subscriber) override;

		private:
			rscpp::Processor<T, std::size_t> &m_parent;
			rscpp::Publisher<T>				  m_publisher;
			rscpp::Subscriber<std::size_t>	  m_subscriber;
			rscpp::Subscription				  m_subscription;
			std::atomic_size_t				  m_count = 0;
		};

	public:
		/**
		 * @brief Construct a new {@link Count} instance.
		 *
		 * @param publisher The source {@link rscpp::Publisher} the {@link Count} {@link rscpp::Processor} subscribes to.
		 */
		explicit Count(const rscpp::Publisher<T> &publisher);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/Count.inl>
