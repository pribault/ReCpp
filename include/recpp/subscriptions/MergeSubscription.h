#pragma once

#include <rscpp/Publisher.h>
#include <rscpp/Subscriber.h>
#include <rscpp/Subscription.h>

#include <map>

namespace recpp::subscribers
{
	template <typename T>
	class DefaultSubscriber;
}

namespace recpp::subscriptions
{
	/**
	 * @class MergeSubscription MergeSubscription.h <recpp/subscriptions/MergeSubscription.h>
	 * @brief {@link rscpp::Subscription} that will emit a range of items from other {@link rscpp::Publisher} instances when requested.
	 *
	 * @tparam T The type of element signaled to the {@link rscpp::Subscriber}.
	 * @tparam I The type of the range iterator.
	 */
	template <typename T, class I>
	class MergeSubscription : public rscpp::Subscription
	{
		class Impl : public rscpp::Subscription
		{
		public:
			Impl(const rscpp::Subscriber<T> &subscriber, I first, I last);

			void request(size_t count) override;
			void cancel() override;

		private:
			void onPublisherNextValue(const T &value);
			void onPublisherError(size_t publisherId, const std::exception_ptr &exceptionPtr);
			void onPublisherComplete(size_t publisherId);

			rscpp::Subscriber<T>				  m_subscriber;
			std::map<size_t, rscpp::Subscription> m_subscriptions;
			int									  m_currentIndex = 0;
			bool								  m_completed = false;
			bool								  m_canceled = false;
			size_t								  m_remaining = 0;
		};

	public:
		/**
		 * @brief Construct a new {@link MergeSubscription} instance.
		 *
		 * @param subscriber The {@link rscpp::Subscriber} that will consume signals from this {@link rscpp::Subscription}.
		 * @param first The first iterator.
		 * @param last The last iterator.
		 */
		MergeSubscription(const rscpp::Subscriber<T> &subscriber, I first, I last);
	};
} // namespace recpp::subscriptions

#include <recpp/subscriptions/inl/MergeSubscription.inl>
