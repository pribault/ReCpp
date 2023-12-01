#pragma once

#include <rscpp/Subscriber.h>
#include <rscpp/Subscription.h>

namespace recpp::subscriptions
{
	template <typename T>
	class JustSubscription : public rscpp::Subscription
	{
		class Impl : public rscpp::Subscription
		{
		public:
			Impl(const rscpp::Subscriber<T> &subscriber, const T &value);

			void request(size_t count) override;

			void cancel() override;

		private:
			rscpp::Subscriber<T> m_subscriber;
			T					 m_value;
			bool				 m_completed = false;
		};

	public:
		JustSubscription(const rscpp::Subscriber<T> &subscriber, const T &value);
	};
} // namespace recpp::subscriptions

#include <recpp/subscriptions/inl/JustSubscription.inl>
