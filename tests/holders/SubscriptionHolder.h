#pragma once

// rscpp
#include <rscpp/Subscription.h>

template <typename T>
class SubscriptionHolder : public rscpp::Subscription
{
public:
	explicit SubscriptionHolder()
		: rscpp::Subscription(std::make_shared<T>())
	{
	}

	std::shared_ptr<T> dPtr() const
	{
		return static_pointer_cast<T>(d_ptr);
	}
};
