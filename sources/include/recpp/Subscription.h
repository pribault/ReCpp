#pragma once

#include <rscpp/Subscriber.h>
#include <rscpp/Subscription.h>

namespace recpp
{
	template <typename T>
	class Subscription : public rscpp::Subscription
	{
	public:
		Subscription(const typename rscpp::Subscriber<T> &subscriber, const typename rscpp::Subscription::RequestMethod &requestMethod,
					 const typename rscpp::Subscription::CancelMethod &cancelMethod, const rscpp::Subscription::StatePtr &state = nullptr);

	protected:
		rscpp::Subscriber<T> m_subscriber;
	};
} // namespace recpp

#include <recpp/Subscription.inl>
