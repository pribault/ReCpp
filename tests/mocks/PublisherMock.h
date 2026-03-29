#pragma once

// rscpp
#include <rscpp/Publisher.h>

// gmock
#include <gmock/gmock.h>

template <typename T>
class PublisherMock : public rscpp::Publisher<T>
{
public:
	MOCK_METHOD(void, subscribe, (rscpp::Subscriber<T> &), (override));
};
