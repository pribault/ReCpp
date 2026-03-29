#pragma once

// rscpp
#include <rscpp/Subscriber.h>

// gmock
#include <gmock/gmock.h>

template <typename T>
class SubscriberMock : public rscpp::Subscriber<T>
{
public:
	MOCK_METHOD(void, onSubscribe, (rscpp::Subscription &), (override));
	MOCK_METHOD(void, onNext, (const T &), (override));
	MOCK_METHOD(void, onError, (const std::exception_ptr &), (override));
	MOCK_METHOD(void, onComplete, (), (override));
};
