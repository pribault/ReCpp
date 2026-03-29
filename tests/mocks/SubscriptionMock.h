#pragma once

// rscpp
#include <rscpp/Subscription.h>

// gmock
#include <gmock/gmock.h>

class SubscriptionMock : public rscpp::Subscription
{
public:
	MOCK_METHOD(void, request, (size_t), (override));
	MOCK_METHOD(void, cancel, (), (override));
};
