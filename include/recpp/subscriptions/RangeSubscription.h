#pragma once

#include <rscpp/Subscriber.h>
#include <rscpp/Subscription.h>

namespace recpp
{
	template <typename T, class I>
	class RangeSubscription : public rscpp::Subscription
	{
		class Impl : public rscpp::Subscription
		{
		public:
			Impl(const rscpp::Subscriber<T> &subscriber, I first, I last);

			void request(size_t count) override;

			void cancel() override;

		private:
			rscpp::Subscriber<T> m_subscriber;
			I					 m_current;
			I					 m_last;
			bool				 m_completed = false;
			bool				 m_canceled = false;
		};

	public:
		RangeSubscription(const rscpp::Subscriber<T> &subscriber, I first, I last);
	};
} // namespace recpp

#include <recpp/subscriptions/inl/RangeSubscription.inl>
