#pragma once

#include <rscpp/Processor.h>

#include <recpp/subscriptions/FilterSubscription.h>

#include <functional>
#include <vector>

namespace recpp::processors
{
	/**
	 * @class TakeWhile TakeWhile.h <recpp/processors/TakeWhile.h>
	 * @brief {@link rscpp::Processor} that will subscribe to a given {@link rscpp::Publisher} and emits items as long as they satisfy the given predicate.
	 *
	 * @tparam T The type of element signaled to the {@link rscpp::Subscriber} and signaled from the {@link rscpp::Publisher}.
	 */
	template <typename T>
	class TakeWhile : public rscpp::Processor<T, T>
	{
		class Impl : public rscpp::Processor<T, T>
		{
		public:
			explicit Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, const std::function<bool(const T & /* value */)> &predicate);

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
			std::function<bool(const T & /* value */)>			  m_predicate = 0;
		};

	public:
		/**
		 * @brief Construct a new {@link TakeWhile} instance.
		 *
		 * @param publisher The source {@link rscpp::Publisher} the {@link TakeWhile} {@link rscpp::Processor} subscribes to and filter the items.
		 * @param predicate The maximum number of items to emit.
		 */
		explicit TakeWhile(const rscpp::Publisher<T> &publisher, const std::function<bool(const T & /* value */)> &predicate);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/TakeWhile.inl>
