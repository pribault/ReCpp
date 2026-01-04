#pragma once

#include <rscpp/Processor.h>

#include <recpp/subscriptions/FilterSubscription.h>

#include <atomic>
#include <functional>
#include <mutex>
#include <vector>

namespace recpp::processors
{
	/**
	 * @class Take Take.h <recpp/processors/Take.h>
	 * @brief {@link rscpp::Processor} that will subscribe to a given {@link rscpp::Publisher} and emits only the first count items emitted.
	 *
	 * @tparam T The type of element signaled to the {@link rscpp::Subscriber} and signaled from the {@link rscpp::Publisher}.
	 */
	template <typename T>
	class Take : public rscpp::Processor<T, T>
	{
		class Impl : public rscpp::Processor<T, T>
		{
		public:
			explicit Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, std::size_t count);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<T> &subscriber) override;

		private:
			rscpp::Processor<T, T>								 &m_parent;
			rscpp::Publisher<T>									  m_publisher;
			std::vector<recpp::subscriptions::FilterSubscription> m_subscriptions;
			rscpp::Subscriber<T>								  m_subscriber;
			std::mutex											  m_mutex;
			std::size_t											  m_count = 0;
		};

	public:
		/**
		 * @brief Construct a new {@link Take} instance.
		 *
		 * @param publisher The source {@link rscpp::Publisher} the {@link Take} {@link rscpp::Processor} subscribes to and filter the items.
		 * @param count The maximum number of items to emit.
		 */
		explicit Take(const rscpp::Publisher<T> &publisher, std::size_t count);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/Take.inl>
