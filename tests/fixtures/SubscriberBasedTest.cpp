#include "fixtures/SubscriberBasedTest.h"

#include "holders/PublisherHolder.h"
#include "holders/SubscriptionHolder.h"
#include "mocks/PublisherMock.h"
#include "mocks/SubscriptionMock.h"

using namespace testing;

void SubscriberBasedTest::checkAll()
{
	checkFetchValues();
	checkSubscriptionIsReleasedAfterCompletion();
	checkSubscriptionIsReleasedAfterError();
}

void SubscriberBasedTest::checkFetchValues()
{
	SubscriptionHolder<SubscriptionMock> subscription;
	EXPECT_CALL(*subscription.dPtr(), request(_)).Times(AtMost(1));

	auto subscriber = this->subscriber();
	subscriber.onSubscribe(subscription);
}

void SubscriberBasedTest::checkSubscriptionIsReleasedAfterCompletion()
{
	PublisherHolder<PublisherMock<int>>	 publisher;
	SubscriptionHolder<SubscriptionMock> subscription;
	EXPECT_CALL(*subscription.dPtr(), request(_)).Times(AtMost(1));
	EXPECT_CALL(*publisher.dPtr(), subscribe(_))
		.WillOnce(
			[&subscription](auto &subscriber)
			{
				subscriber.onSubscribe(subscription);
			});

	auto							subscriber = this->subscriber();
	std::weak_ptr<SubscriptionMock> ptr = subscription.dPtr();
	EXPECT_EQ(ptr.use_count(), 1);
	publisher.subscribe(subscriber);
	EXPECT_GE(ptr.use_count(), 1);
	wait();
	subscriber.onComplete();
	EXPECT_EQ(ptr.use_count(), 1);
}

void SubscriberBasedTest::checkSubscriptionIsReleasedAfterError()
{
	PublisherHolder<PublisherMock<int>>	 publisher;
	SubscriptionHolder<SubscriptionMock> subscription;
	EXPECT_CALL(*subscription.dPtr(), request(_)).Times(AtMost(1));
	EXPECT_CALL(*publisher.dPtr(), subscribe(_))
		.WillOnce(
			[&subscription](auto &subscriber)
			{
				subscriber.onSubscribe(subscription);
			});

	auto							subscriber = this->subscriber();
	std::weak_ptr<SubscriptionMock> ptr = subscription.dPtr();
	EXPECT_EQ(ptr.use_count(), 1);
	publisher.subscribe(subscriber);
	EXPECT_GE(ptr.use_count(), 1);
	wait();
	subscriber.onError(std::runtime_error("runtime_error"));
	EXPECT_EQ(ptr.use_count(), 1);
}

void SubscriberBasedTest::wait() const {}
