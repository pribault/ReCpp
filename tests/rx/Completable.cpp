// gtest
#include <gtest/gtest.h>

// recpp
#include <recpp/async/WorkerThread.h>
#include <recpp/rx/Completable.h>
#include <recpp/rx/Maybe.h>
#include <recpp/rx/Observable.h>
#include <recpp/rx/Single.h>

// stl
#include <array>
#include <iostream>

using namespace recpp::async;
using namespace recpp::rx;
using namespace std;

namespace
{
	constexpr auto				 sleepDuration = chrono::milliseconds(10);
	constexpr int				 defaultValue = 42;
	constexpr std::array<int, 3> defaultValues({1, 2, 3});
	constexpr auto				 delayTolerance = chrono::milliseconds(10);
	constexpr auto				 delayDuration = chrono::milliseconds(100);
	constexpr auto				 sleepDurationForDelay = chrono::milliseconds(150);
} // namespace

TEST(Completable, complete)
{
	bool completed = false;
	EXPECT_NO_THROW(Completable::complete().subscribe(
		[&completed]()
		{
			if (completed)
				throw runtime_error("completion handler called twice");
			completed = true;
		},
		[](const auto &exception)
		{
			throw runtime_error("error handler called");
		}));
	EXPECT_TRUE(completed);
}

TEST(Completable, create)
{
	bool completed = false;
	EXPECT_NO_THROW(Completable::create(
						[](auto &subscriber)
						{
							subscriber.onComplete();
							// Try to complete again, should not be forwarded
							subscriber.onComplete();
							// Try to send an error, should not be forwared
							subscriber.onError(make_exception_ptr(runtime_error("unexpected error!")));
						})
						.subscribe(
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							},
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							}));
	EXPECT_TRUE(completed);

	bool errored = false;
	EXPECT_NO_THROW(Completable::create(
						[](auto &subscriber)
						{
							subscriber.onError(make_exception_ptr(runtime_error("unexpected error!")));
							// Try to emit another error, should not be forwarded
							subscriber.onError(make_exception_ptr(runtime_error("unexpected error!")));
							// Try to complete, should not be forwarded
							subscriber.onComplete();
						})
						.subscribe(
							[]()
							{
								throw runtime_error("completion handler called");
							},
							[&errored](const auto &exception)
							{
								if (errored)
									throw runtime_error("error handler called twice");
								errored = true;
							}));
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Completable::create([](auto &subscriber) {})
						.subscribe(
							[]()
							{
								throw runtime_error("completion handler called");
							},
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							}));
}

TEST(Completable, defer)
{
	bool completed = false;
	EXPECT_NO_THROW(Completable::defer(
						[]()
						{
							return Completable::complete();
						})
						.subscribe(
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							},
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							}));
	EXPECT_TRUE(completed);

	bool errored = false;
	EXPECT_NO_THROW(Completable::defer(
						[]()
						{
							return Completable::error(make_exception_ptr(runtime_error("unexpected error!")));
						})
						.subscribe(
							[]()
							{
								throw runtime_error("completion handler called");
							},
							[&errored](const auto &exception)
							{
								if (errored)
									throw runtime_error("error handler called twice");
								errored = true;
							}));
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Completable::defer(
						[]()
						{
							return Completable::never();
						})
						.subscribe(
							[]()
							{
								throw runtime_error("completion handler called");
							},
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							}));
}

TEST(Completable, error)
{
	bool errored = false;
	EXPECT_NO_THROW(Completable::error(make_exception_ptr(runtime_error("unexpected error!")))
						.subscribe(
							[]()
							{
								throw runtime_error("completion handler called");
							},
							[&errored](const auto &exception)
							{
								if (errored)
									throw runtime_error("error handler called twice");
								errored = true;
							}));
	EXPECT_TRUE(errored);
}

TEST(Completable, never)
{
	EXPECT_NO_THROW(Completable::never().subscribe(
		[]()
		{
			throw runtime_error("completion handler called");
		},
		[](const auto &exception)
		{
			throw runtime_error("error handler called");
		}));
}

TEST(Completable, doOnComplete)
{
	bool completed = false;
	EXPECT_NO_THROW(Completable::complete()
						.doOnComplete(
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							})
						.subscribe());
	EXPECT_TRUE(completed);

	EXPECT_NO_THROW(Completable::error(make_exception_ptr(runtime_error("unexpected error!")))
						.doOnComplete(
							[]()
							{
								throw runtime_error("completion handler called");
							})
						.subscribe());

	EXPECT_NO_THROW(Completable::never()
						.doOnComplete(
							[]()
							{
								throw runtime_error("completion handler called");
							})
						.subscribe());
}

TEST(Completable, doOnError)
{
	bool errored = false;
	EXPECT_NO_THROW(Completable::error(make_exception_ptr(runtime_error("unexpected error!")))
						.doOnError(
							[&errored](const auto &exception)
							{
								if (errored)
									throw runtime_error("error handler called twice");
								errored = true;
							})
						.subscribe());
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Completable::complete()
						.doOnError(
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							})
						.subscribe());

	EXPECT_NO_THROW(Completable::never()
						.doOnError(
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							})
						.subscribe());
}

TEST(Completable, doOnTerminate)
{
	bool terminated = false;
	EXPECT_NO_THROW(Completable::complete()
						.doOnTerminate(
							[&terminated]()
							{
								if (terminated)
									throw runtime_error("termination handler called twice");
								terminated = true;
							})
						.subscribe());
	EXPECT_TRUE(terminated);

	terminated = false;
	EXPECT_NO_THROW(Completable::error(make_exception_ptr(runtime_error("unexpected error!")))
						.doOnTerminate(
							[&terminated]()
							{
								if (terminated)
									throw runtime_error("termination handler called twice");
								terminated = true;
							})
						.subscribe());
	EXPECT_TRUE(terminated);

	EXPECT_NO_THROW(Completable::never()
						.doOnTerminate(
							[]()
							{
								throw runtime_error("termination handler called");
							})
						.subscribe());
}

TEST(Completable, tap)
{
	bool completed = false;
	EXPECT_NO_THROW(Completable::complete()
						.tap(
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							},
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							})
						.subscribe());
	EXPECT_TRUE(completed);

	bool errored = false;
	EXPECT_NO_THROW(Completable::error(make_exception_ptr(runtime_error("unexpected error!")))
						.tap(
							[]()
							{
								throw runtime_error("completion handler called");
							},
							[&errored](const auto &exception)
							{
								if (errored)
									throw runtime_error("error handler called twice");
								errored = true;
							})
						.subscribe());
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Completable::never()
						.tap(
							[]()
							{
								throw runtime_error("completion handler called");
							},
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							})
						.subscribe());
}

TEST(Completable, observeOn)
{
	WorkerThread worker;
	const auto	 workerThreadId = worker.threadId();
	const auto	 mainThreadId = this_thread::get_id();

	bool completed = false;
	bool testFailed = false;
	Completable::complete()
		.doOnComplete(
			[&testFailed, mainThreadId]()
			{
				if (this_thread::get_id() != mainThreadId)
					testFailed = true;
			})
		.observeOn(worker)
		.subscribe(
			[&completed, &testFailed, workerThreadId]()
			{
				if (completed)
					testFailed = true;
				completed = true;
				if (this_thread::get_id() != workerThreadId)
					testFailed = true;
			},
			[&testFailed](const auto &exception)
			{
				testFailed = true;
			});
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(completed);
	EXPECT_FALSE(testFailed);

	bool errored = false;
	testFailed = false;
	Completable::error(make_exception_ptr(runtime_error("unexpected error!")))
		.doOnError(
			[&testFailed, mainThreadId](const auto &exception)
			{
				if (this_thread::get_id() != mainThreadId)
					testFailed = true;
			})
		.observeOn(worker)
		.subscribe(
			[&testFailed]()
			{
				testFailed = true;
			},
			[&errored, &testFailed, workerThreadId](const auto &exception)
			{
				if (errored)
					testFailed = true;
				errored = true;
				if (this_thread::get_id() != workerThreadId)
					testFailed = true;
			});
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(errored);
	EXPECT_FALSE(testFailed);
}

TEST(Completable, subscribeOn)
{
	WorkerThread worker;
	const auto	 workerThreadId = worker.threadId();
	const auto	 mainThreadId = this_thread::get_id();

	bool completed = false;
	bool testFailed = false;
	Completable::complete()
		.doOnComplete(
			[&testFailed, workerThreadId]()
			{
				if (this_thread::get_id() != workerThreadId)
					testFailed = true;
			})
		.subscribeOn(worker)
		.subscribe(
			[&completed, &testFailed, workerThreadId]()
			{
				if (completed)
					testFailed = true;
				completed = true;
				if (this_thread::get_id() != workerThreadId)
					testFailed = true;
			},
			[&testFailed](const auto &exception)
			{
				testFailed = true;
			});
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(completed);
	EXPECT_FALSE(testFailed);

	bool errored = false;
	testFailed = false;
	Completable::error(make_exception_ptr(runtime_error("unexpected error!")))
		.doOnError(
			[&testFailed, workerThreadId](const auto &exception)
			{
				if (this_thread::get_id() != workerThreadId)
					testFailed = true;
			})
		.subscribeOn(worker)
		.subscribe(
			[&testFailed]()
			{
				testFailed = true;
			},
			[&errored, &testFailed, workerThreadId](const auto &exception)
			{
				if (errored)
					testFailed = true;
				errored = true;
				if (this_thread::get_id() != workerThreadId)
					testFailed = true;
			});
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(errored);
	EXPECT_FALSE(testFailed);
}

TEST(Completable, andThen)
{
	bool errored = false;
	EXPECT_NO_THROW(Completable::error(make_exception_ptr(runtime_error("unexpected error!")))
						.andThen(Completable::complete())
						.subscribe(
							[]()
							{
								throw runtime_error("completion handler called");
							},
							[&errored](const auto &exception)
							{
								if (errored)
									throw runtime_error("error handler called twice");
								errored = true;
							}));
	EXPECT_TRUE(errored);

	errored = false;
	EXPECT_NO_THROW(Completable::complete()
						.andThen(Completable::error(make_exception_ptr(runtime_error("unexpected error!"))))
						.subscribe(
							[]()
							{
								throw runtime_error("completion handler called");
							},
							[&errored](const auto &exception)
							{
								if (errored)
									throw runtime_error("error handler called twice");
								errored = true;
							}));
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Completable::never()
						.andThen(Completable::complete())
						.subscribe(
							[]()
							{
								throw runtime_error("completion handler called");
							},
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							}));

	EXPECT_NO_THROW(Completable::never()
						.andThen(Completable::error(make_exception_ptr(runtime_error("unexpected error!"))))
						.subscribe(
							[]()
							{
								throw runtime_error("completion handler called");
							},
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							}));

	bool deferCalled = false;
	bool completed = false;
	EXPECT_NO_THROW(Completable::complete()
						.andThen(Completable::defer(
							[&deferCalled]()
							{
								if (deferCalled)
									throw runtime_error("defer handler called twice");
								deferCalled = true;
								return Completable::complete();
							}))
						.subscribe(
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							},
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							}));
	EXPECT_TRUE(deferCalled);
	EXPECT_TRUE(completed);

	deferCalled = false;
	errored = false;
	EXPECT_NO_THROW(Completable::complete()
						.andThen(Completable::defer(
							[&deferCalled]()
							{
								if (deferCalled)
									throw runtime_error("defer handler called twice");
								deferCalled = true;
								return Completable::error(make_exception_ptr(runtime_error("unexpected error!")));
							}))
						.subscribe(
							[]()
							{
								throw runtime_error("completion handler called");
							},
							[&errored](const auto &exception)
							{
								if (errored)
									throw runtime_error("error handler called twice");
								errored = true;
							}));
	EXPECT_TRUE(deferCalled);
	EXPECT_TRUE(errored);

	deferCalled = false;
	EXPECT_NO_THROW(Completable::complete()
						.andThen(Completable::defer(
							[&deferCalled]()
							{
								if (deferCalled)
									throw runtime_error("defer handler called twice");
								deferCalled = true;
								return Completable::never();
							}))
						.subscribe(
							[]()
							{
								throw runtime_error("completion handler called");
							},
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							}));
	EXPECT_TRUE(deferCalled);

	bool succeeded = false;
	completed = false;
	EXPECT_NO_THROW(Completable::complete()
						.andThen(Maybe<float>::just(defaultValue))
						.subscribe(
							[&succeeded](const auto value)
							{
								if (succeeded)
									throw runtime_error("success handler called twice");
								if (value != defaultValue)
									throw runtime_error("invalid value");
								succeeded = true;
							},
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							},
							[&completed]()
							{
								if (completed)
									throw runtime_error("success handler called twice");
								completed = true;
							}));
	EXPECT_TRUE(succeeded);
	EXPECT_TRUE(completed);

	succeeded = false;
	EXPECT_NO_THROW(Completable::complete()
						.andThen(Single<float>::just(defaultValue))
						.subscribe(
							[&succeeded](const auto value)
							{
								if (succeeded)
									throw runtime_error("success handler called twice");
								if (value != defaultValue)
									throw runtime_error("invalid value");
								succeeded = true;
							},
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							}));
	EXPECT_TRUE(succeeded);

	size_t valuesCount = 0;
	completed = false;
	EXPECT_NO_THROW(Completable::complete()
						.andThen(Observable<int>::range(defaultValues))
						.subscribe(
							[&valuesCount](const auto value)
							{
								if (valuesCount >= defaultValues.size())
									throw runtime_error("too much values forwarded");
								const auto expectedValue = defaultValues[valuesCount++];
								if (value != expectedValue)
									throw runtime_error("unexpected value");
							},
							[](const auto &exception)
							{
								throw runtime_error("error handler called");
							},
							[&completed]()
							{
								if (completed)
									throw runtime_error("success handler called twice");
								completed = true;
							}));
	EXPECT_TRUE(completed);
	EXPECT_EQ(valuesCount, defaultValues.size());
}

TEST(Completable, delay)
{
	WorkerThread worker;
	const auto	 workerThreadId = worker.threadId();
	const auto	 mainThreadId = this_thread::get_id();

	bool				completed = false;
	bool				testFailed = false;
	auto				startTime = SchedulableQueue::Clock::now();
	decltype(startTime) afterDelayTime;
	Completable::complete()
		.doOnComplete(
			[&testFailed, mainThreadId]()
			{
				if (this_thread::get_id() != mainThreadId)
					testFailed = true;
			})
		.delay(worker, delayDuration, true)
		.subscribe(
			[&completed, &testFailed, &afterDelayTime, workerThreadId]()
			{
				if (completed)
					testFailed = true;
				completed = true;
				if (this_thread::get_id() != workerThreadId)
					testFailed = true;
				afterDelayTime = SchedulableQueue::Clock::now();
			},
			[&testFailed](const auto &exception)
			{
				testFailed = true;
			});
	this_thread::sleep_for(sleepDurationForDelay);
	EXPECT_TRUE(completed);
	EXPECT_FALSE(testFailed);
	auto timeDiff = afterDelayTime - startTime;
	auto gap = timeDiff - delayDuration;
	EXPECT_LT(chrono::abs(gap), delayTolerance);

	bool errored = false;
	testFailed = false;
	startTime = SchedulableQueue::Clock::now();
	Completable::error(make_exception_ptr(runtime_error("unexpected error!")))
		.doOnError(
			[&testFailed, mainThreadId](const auto &exception)
			{
				if (this_thread::get_id() != mainThreadId)
					testFailed = true;
			})
		.delay(worker, delayDuration, true)
		.subscribe(
			[&testFailed]()
			{
				testFailed = true;
			},
			[&errored, &testFailed, &afterDelayTime, workerThreadId](const auto &exception)
			{
				if (errored)
					testFailed = true;
				errored = true;
				if (this_thread::get_id() != workerThreadId)
					testFailed = true;
				afterDelayTime = SchedulableQueue::Clock::now();
			});
	this_thread::sleep_for(sleepDurationForDelay);
	EXPECT_TRUE(errored);
	EXPECT_FALSE(testFailed);
	timeDiff = afterDelayTime - startTime;
	gap = timeDiff - delayDuration;
	EXPECT_LT(chrono::abs(gap), delayTolerance);
}
