#pragma once

#include <rscpp/Processor.h>

#include <functional>

namespace recpp::processors
{
	/**
	 * @class AllOf AllOf.h <recpp/processors/AllOf.h>
	 * @brief {@link rscpp::Processor} that will subscribe to a given {@link rscpp::Publisher} check that a given predicate returns true for all emited values.
	 *
	 * @param T The type of element signaled to the {@link rscpp::Subscriber}.
	 * @param R The type of the predicate to call on each value.
	 */
	template <typename T, typename R>
	class AllOf : public rscpp::Processor<T, bool>
	{
		class Impl : public rscpp::Processor<T, bool>
		{
		public:
			explicit Impl(rscpp::Processor<T, bool> &parent, const rscpp::Publisher<T> &publisher, R predicate);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<bool> &subscriber) override;

		private:
			rscpp::Processor<T, bool> &m_parent;
			rscpp::Publisher<T>		   m_publisher;
			rscpp::Subscriber<bool>	   m_subscriber;
			rscpp::Subscription		   m_subscription;
			R						   m_predicate;
			bool					   m_gotFalse = false;
		};

	public:
		/**
		 * @brief Construct a new {@link AllOf} instance.
		 *
		 * @param publisher The source {@link rscpp::Publisher} the {@link AllOf} {@link rscpp::Processor} subscribes to.
		 * @param predicate The predicate to call on each value.
		 */
		explicit AllOf(const rscpp::Publisher<T> &publisher, R predicate);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/AllOf.inl>
