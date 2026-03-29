#pragma once

// rscpp
#include <rscpp/Subscriber.h>

// gtest
#include <gtest/gtest.h>

class SubscriberBasedTest : public testing::Test
{
public:
	void checkAll();
	void checkFetchValues();
	void checkSubscriptionIsReleasedAfterCompletion();
	void checkSubscriptionIsReleasedAfterError();

	virtual rscpp::Subscriber<int> subscriber() const = 0;
	virtual void				   wait() const;
};
