#include <gtest/gtest.h>
#include <iostream>
#include <recpp/async/WorkerThread.h>
#include <recpp/rx/Single.h>

using namespace recpp;
using namespace std;

namespace
{
	constexpr int defaultValue = 42;
} // namespace

TEST(Single, create)
{
	bool completed = false;
	EXPECT_NO_THROW(Single<int>::create(
						[](auto &subscriber)
						{
							subscriber.onNext(defaultValue);
							// Try to complete again, should not be forwarded
							subscriber.onNext(defaultValue);
							// Try to send an error, should not be forwared
							subscriber.onError(make_exception_ptr(runtime_error("unexpected error!")));
						})
						.subscribe(
							[&completed](const auto value)
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								if (value != defaultValue)
									throw runtime_error("invalid value");
								completed = true;
							},
							[](const auto &exception) { throw runtime_error("error handler called"); }));
	EXPECT_TRUE(completed);

	bool errored = false;
	EXPECT_NO_THROW(Single<int>::create(
						[](auto &subscriber)
						{
							subscriber.onError(make_exception_ptr(runtime_error("unexpected error!")));
							// Try to emit another error, should not be forwarded
							subscriber.onError(make_exception_ptr(runtime_error("unexpected error!")));
							// Try to complete, should not be forwarded
							subscriber.onNext(defaultValue);
						})
						.subscribe([](const auto value) { throw runtime_error("completion handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   }));
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Single<int>::create([](auto &subscriber) {})
						.subscribe([](const auto value) { throw runtime_error("completion handler called"); },
								   [](const auto &exception) { throw runtime_error("error handler called"); }));
}

TEST(Single, defer)
{
	bool completed = false;
	EXPECT_NO_THROW(Single<int>::defer([]() { return Single<int>::just(defaultValue); })
						.subscribe(
							[&completed](const auto value)
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								if (value != defaultValue)
									throw runtime_error("invalid value");
								completed = true;
							},
							[](const auto &exception) { throw runtime_error("error handler called"); }));
	EXPECT_TRUE(completed);

	bool errored = false;
	EXPECT_NO_THROW(Single<int>::defer([]() { return Single<int>::error(make_exception_ptr(runtime_error("unexpected error!"))); })
						.subscribe([](const auto value) { throw runtime_error("completion handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   }));
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Single<int>::defer([]() { return Single<int>::never(); })
						.subscribe([](const auto value) { throw runtime_error("completion handler called"); },
								   [](const auto &exception) { throw runtime_error("error handler called"); }));
}

TEST(Single, error)
{
	bool errored = false;
	EXPECT_NO_THROW(Single<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.subscribe([](const auto value) { throw runtime_error("completion handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   }));
	EXPECT_TRUE(errored);
}

TEST(Single, just)
{
	bool completed = false;
	EXPECT_NO_THROW(Single<int>::just(defaultValue)
						.subscribe(
							[&completed](const auto value)
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								if (value != defaultValue)
									throw runtime_error("invalid value");
								completed = true;
							},
							[](const auto &exception) { throw runtime_error("error handler called"); }));
	EXPECT_TRUE(completed);
}

TEST(Single, never)
{
	EXPECT_NO_THROW(Single<int>::never().subscribe([](const auto value) { throw runtime_error("completion handler called"); },
												   [](const auto &exception) { throw runtime_error("error handler called"); }));
}

TEST(Single, map)
{
	bool completed = false;
	EXPECT_NO_THROW(Single<int>::just(defaultValue)
						.map<float>([](const auto value) { return static_cast<float>(value) / 1000; })
						.subscribe(
							[&completed](const auto value)
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								if (value != static_cast<float>(defaultValue) / 1000)
									throw runtime_error("invalid value");
								completed = true;
							},
							[](const auto &exception) { throw runtime_error("error handler called"); }));
	EXPECT_TRUE(completed);

	bool errored = false;
	EXPECT_NO_THROW(Single<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.map<float>([](const auto value) { return static_cast<float>(value) / 1000; })
						.subscribe([](const auto value) { throw runtime_error("completion handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   }));
	EXPECT_TRUE(errored);
}

TEST(Single, flatMap)
{
	bool completed = false;
	EXPECT_NO_THROW(Single<int>::just(defaultValue)
						.flatMap<float>([](const auto value) { return Single<float>::just(static_cast<float>(value) / 1000); })
						.subscribe(
							[&completed](const auto value)
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								if (value != static_cast<float>(defaultValue) / 1000)
									throw runtime_error("invalid value");
								completed = true;
							},
							[](const auto &exception) { throw runtime_error("error handler called"); }));
	EXPECT_TRUE(completed);

	bool errored = false;
	EXPECT_NO_THROW(Single<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.flatMap<float>([](const auto value) { return Single<float>::just(static_cast<float>(value) / 1000); })
						.subscribe([](const auto value) { throw runtime_error("completion handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   }));
	EXPECT_TRUE(errored);

	errored = false;
	EXPECT_NO_THROW(Single<int>::just(defaultValue)
						.flatMap<float>([](const auto value) { return Single<float>::error(make_exception_ptr(runtime_error("unexpected error!"))); })
						.subscribe([](const auto value) { throw runtime_error("completion handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   }));
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Single<int>::just(defaultValue)
						.flatMap<float>([](const auto value) { return Single<float>::never(); })
						.subscribe([](const auto value) { throw runtime_error("completion handler called"); },
								   [&errored](const auto &exception) { throw runtime_error("error handler called"); }));
}

TEST(Single, doOnError)
{
	bool errored = false;
	EXPECT_NO_THROW(Single<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.doOnError(
							[&errored](const auto &exception)
							{
								if (errored)
									throw runtime_error("error handler called twice");
								errored = true;
							})
						.subscribe());
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Single<int>::just(defaultValue).doOnError([](const auto &exception) { throw runtime_error("error handler called"); }).subscribe());

	EXPECT_NO_THROW(Single<int>::never().doOnError([](const auto &exception) { throw runtime_error("error handler called"); }).subscribe());
}

TEST(Single, doOnSuccess)
{
	bool succeeded = false;
	EXPECT_NO_THROW(Single<int>::just(defaultValue)
						.doOnSuccess(
							[&succeeded](const auto value)
							{
								if (succeeded)
									throw runtime_error("success handler called twice");
								succeeded = true;
							})
						.subscribe());
	EXPECT_TRUE(succeeded);

	EXPECT_NO_THROW(Single<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.doOnSuccess([&succeeded](const auto value) { throw runtime_error("success handler called"); })
						.subscribe());

	EXPECT_NO_THROW(Single<int>::never().doOnSuccess([](const auto value) { throw runtime_error("success handler called"); }).subscribe());
}

TEST(Single, doOnTerminate)
{
	bool terminated = false;
	EXPECT_NO_THROW(Single<int>::just(defaultValue)
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
	EXPECT_NO_THROW(Single<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.doOnTerminate(
							[&terminated]()
							{
								if (terminated)
									throw runtime_error("termination handler called twice");
								terminated = true;
							})
						.subscribe());
	EXPECT_TRUE(terminated);

	EXPECT_NO_THROW(Single<int>::never().doOnTerminate([]() { throw runtime_error("terminate handler called"); }).subscribe());
}

TEST(Single, tap)
{
	bool succeeded = false;
	EXPECT_NO_THROW(Single<int>::just(defaultValue)
						.tap(
							[&succeeded](const auto value)
							{
								if (succeeded)
									throw runtime_error("success handler called twice");
								if (value != defaultValue)
									throw runtime_error("invalid value");
								succeeded = true;
							},
							[](const auto &exception) { throw runtime_error("error handler called"); })
						.subscribe());
	EXPECT_TRUE(succeeded);

	bool errored = false;
	EXPECT_NO_THROW(Single<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.tap([](const auto value) { throw runtime_error("success handler called"); },
							 [&errored](const auto &exception)
							 {
								 if (errored)
									 throw runtime_error("error handler called twice");
								 errored = true;
							 })
						.subscribe());
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Single<int>::never()
						.tap([](const auto value) { throw runtime_error("completion handler called"); },
							 [](const auto &exception) { throw runtime_error("error handler called"); })
						.subscribe());
}

TEST(Single, observeOn)
{
	WorkerThread worker;
	const auto	 workerThreadId = worker.threadId();
	const auto	 mainThreadId = this_thread::get_id();

	bool completed = false;
	bool testFailed = false;
	Single<int>::just(defaultValue)
		.doOnSuccess(
			[&testFailed, mainThreadId](const auto value)
			{
				if (this_thread::get_id() != mainThreadId)
					testFailed = true;
			})
		.observeOn(worker)
		.subscribe(
			[&completed, &testFailed, workerThreadId](const auto value)
			{
				if (completed)
					testFailed = true;
				completed = true;
				if (this_thread::get_id() != workerThreadId)
					testFailed = true;
				if (value != defaultValue)
					testFailed = true;
			},
			[&testFailed](const auto &exception) { testFailed = true; });
	this_thread::sleep_for(chrono::milliseconds(2));
	EXPECT_TRUE(completed);
	EXPECT_FALSE(testFailed);

	bool errored = false;
	testFailed = false;
	Single<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
		.doOnError(
			[&testFailed, mainThreadId](const auto &exception)
			{
				if (this_thread::get_id() != mainThreadId)
					testFailed = true;
			})
		.observeOn(worker)
		.subscribe([&testFailed](const auto value) { testFailed = true; },
				   [&errored, &testFailed, workerThreadId](const auto &exception)
				   {
					   if (errored)
						   testFailed = true;
					   errored = true;
					   if (this_thread::get_id() != workerThreadId)
						   testFailed = true;
				   });
	this_thread::sleep_for(chrono::milliseconds(2));
	EXPECT_TRUE(errored);
	EXPECT_FALSE(testFailed);
}

TEST(Single, subscribeOn)
{
	WorkerThread worker;
	const auto	 workerThreadId = worker.threadId();
	const auto	 mainThreadId = this_thread::get_id();

	bool completed = false;
	bool testFailed = false;
	Single<int>::just(defaultValue)
		.doOnSuccess(
			[&testFailed, workerThreadId](const auto value)
			{
				if (this_thread::get_id() != workerThreadId)
					testFailed = true;
			})
		.subscribeOn(worker)
		.subscribe(
			[&completed, &testFailed, workerThreadId](const auto value)
			{
				if (completed)
					testFailed = true;
				completed = true;
				if (this_thread::get_id() != workerThreadId)
					testFailed = true;
				if (value != defaultValue)
					testFailed = true;
			},
			[&testFailed](const auto &exception) { testFailed = true; });
	this_thread::sleep_for(chrono::milliseconds(2));
	EXPECT_TRUE(completed);
	EXPECT_FALSE(testFailed);

	bool errored = false;
	testFailed = false;
	Single<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
		.doOnError(
			[&testFailed, workerThreadId](const auto &exception)
			{
				if (this_thread::get_id() != workerThreadId)
					testFailed = true;
			})
		.subscribeOn(worker)
		.subscribe([&testFailed](const auto value) { testFailed = true; },
				   [&errored, &testFailed, workerThreadId](const auto &exception)
				   {
					   if (errored)
						   testFailed = true;
					   errored = true;
					   if (this_thread::get_id() != workerThreadId)
						   testFailed = true;
				   });
	this_thread::sleep_for(chrono::milliseconds(2));
	EXPECT_TRUE(errored);
	EXPECT_FALSE(testFailed);
}
