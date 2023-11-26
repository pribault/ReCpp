#include <gtest/gtest.h>
#include <iostream>
#include <recpp/async/WorkerThread.h>
#include <recpp/rx/Maybe.h>

using namespace recpp;
using namespace std;

namespace
{
	constexpr int  defaultValue = 42;
	constexpr auto sleepDuration = chrono::milliseconds(10);
} // namespace

TEST(Maybe, create)
{
	bool succeeded = false;
	bool completed = false;
	EXPECT_NO_THROW(Maybe<int>::create(
						[](auto &subscriber)
						{
							subscriber.onNext(defaultValue);
							// Try to complete, should not be forwarded
							subscriber.onComplete();
							// Try to complete again, should not be forwarded
							subscriber.onNext(defaultValue);
							// Try to send an error, should not be forwared
							subscriber.onError(make_exception_ptr(runtime_error("unexpected error!")));
						})
						.subscribe(
							[&succeeded](const auto value)
							{
								if (succeeded)
									throw runtime_error("success handler called twice");
								if (value != defaultValue)
									throw runtime_error("invalid value");
								succeeded = true;
							},
							[](const auto &exception) { throw runtime_error("error handler called"); },
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							}));
	EXPECT_TRUE(succeeded);
	EXPECT_TRUE(completed);

	bool errored = false;
	EXPECT_NO_THROW(Maybe<int>::create(
						[](auto &subscriber)
						{
							subscriber.onError(make_exception_ptr(runtime_error("unexpected error!")));
							// Try to emit another error, should not be forwarded
							subscriber.onError(make_exception_ptr(runtime_error("unexpected error!")));
							// Try to complete, should not be forwarded
							subscriber.onNext(defaultValue);
							// Try to complete, should not be forwarded
							subscriber.onComplete();
						})
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   },
								   []() { throw runtime_error("completion handler called"); }));
	EXPECT_TRUE(errored);

	completed = false;
	EXPECT_NO_THROW(Maybe<int>::create([](auto &subscriber) { subscriber.onComplete(); })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [](const auto &exception) { throw runtime_error("error handler called"); },
								   [&completed]()
								   {
									   if (completed)
										   throw runtime_error("completion handler called twice");
									   completed = true;
								   }));
	EXPECT_TRUE(completed);

	EXPECT_NO_THROW(Maybe<int>::create([](auto &subscriber) {})
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [](const auto &exception) { throw runtime_error("error handler called"); },
								   []() { throw runtime_error("completion handler called"); }));
}

TEST(Maybe, defer)
{
	bool errored = false;
	EXPECT_NO_THROW(Maybe<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   },
								   []() { throw runtime_error("completion handler called"); }));
	EXPECT_TRUE(errored);
}

TEST(Maybe, empty)
{
	bool completed = false;
	EXPECT_NO_THROW(Maybe<int>::empty().subscribe([](const auto value) { throw runtime_error("success handler called"); },
												  [](const auto &exception) { throw runtime_error("error handler called"); },
												  [&completed]()
												  {
													  if (completed)
														  throw runtime_error("completion handler called twice");
													  completed = true;
												  }));
	EXPECT_TRUE(completed);
}

TEST(Maybe, error)
{
	bool errored = false;
	EXPECT_NO_THROW(Maybe<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   },
								   []() { throw runtime_error("completion handler called"); }));
	EXPECT_TRUE(errored);
}

TEST(Maybe, just)
{
	bool succeeded = false;
	bool completed = false;
	EXPECT_NO_THROW(Maybe<int>::just(defaultValue)
						.subscribe(
							[&succeeded](const auto value)
							{
								if (succeeded)
									throw runtime_error("success handler called twice");
								if (value != defaultValue)
									throw runtime_error("invalid value");
								succeeded = true;
							},
							[](const auto &exception) { throw runtime_error("error handler called"); },
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							}));
	EXPECT_TRUE(succeeded);
	EXPECT_TRUE(completed);
}

TEST(Maybe, never)
{
	EXPECT_NO_THROW(Maybe<int>::never().subscribe([](const auto value) { throw runtime_error("success handler called"); },
												  [](const auto &exception) { throw runtime_error("error handler called"); },
												  []() { throw runtime_error("completion handler called"); }));
}

TEST(Maybe, map)
{
	bool succeeded = false;
	bool completed = false;
	EXPECT_NO_THROW(Maybe<int>::just(defaultValue)
						.map<float>([](const auto value) { return static_cast<float>(value) / 1000; })
						.subscribe(
							[&succeeded](const auto value)
							{
								if (succeeded)
									throw runtime_error("success handler called twice");
								if (value != static_cast<float>(defaultValue) / 1000)
									throw runtime_error("invalid value");
								succeeded = true;
							},
							[](const auto &exception) { throw runtime_error("error handler called"); },
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							}));
	EXPECT_TRUE(succeeded);
	EXPECT_TRUE(completed);

	bool errored = false;
	EXPECT_NO_THROW(Maybe<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.map<float>([](const auto value) { return static_cast<float>(value) / 1000; })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   },
								   []() { throw runtime_error("completion handler called"); }));
	EXPECT_TRUE(errored);

	completed = false;
	EXPECT_NO_THROW(Maybe<int>::empty()
						.map<float>([](const auto value) { return static_cast<float>(value) / 1000; })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [](const auto &exception) { throw runtime_error("error handler called"); },
								   [&completed]()
								   {
									   if (completed)
										   throw runtime_error("completion handler called twice");
									   completed = true;
								   }));
	EXPECT_TRUE(completed);

	EXPECT_NO_THROW(Maybe<int>::never()
						.map<float>([](const auto value) { return static_cast<float>(value) / 1000; })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [](const auto &exception) { throw runtime_error("error handler called"); },
								   []() { throw runtime_error("completion handler called"); }));
}

TEST(Maybe, flatMap)
{
	bool succeeded = false;
	bool completed = false;
	EXPECT_NO_THROW(Maybe<int>::just(defaultValue)
						.flatMap<float>([](const auto value) { return Maybe<float>::just(static_cast<float>(value) / 1000); })
						.subscribe(
							[&succeeded](const auto value)
							{
								if (succeeded)
									throw runtime_error("success handler called twice");
								if (value != static_cast<float>(defaultValue) / 1000)
									throw runtime_error("invalid value");
								succeeded = true;
							},
							[](const auto &exception) { throw runtime_error("error handler called"); },
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							}));
	EXPECT_TRUE(completed);
	EXPECT_TRUE(succeeded);

	bool errored = false;
	EXPECT_NO_THROW(Maybe<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.flatMap<float>([](const auto value) { return Maybe<float>::just(static_cast<float>(value) / 1000); })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   },
								   []() { throw runtime_error("completion handler called"); }));
	EXPECT_TRUE(errored);

	errored = false;
	EXPECT_NO_THROW(Maybe<int>::just(defaultValue)
						.flatMap<float>([](const auto value) { return Maybe<float>::error(make_exception_ptr(runtime_error("unexpected error!"))); })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   },
								   []() { throw runtime_error("completion handler called"); }));
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Maybe<int>::just(defaultValue)
						.flatMap<float>([](const auto value) { return Maybe<float>::never(); })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [&errored](const auto &exception) { throw runtime_error("error handler called"); },
								   []() { throw runtime_error("completion handler called"); }));

	EXPECT_NO_THROW(Maybe<int>::never()
						.flatMap<float>([](const auto value) { return Maybe<float>::just(defaultValue); })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [&errored](const auto &exception) { throw runtime_error("error handler called"); },
								   []() { throw runtime_error("completion handler called"); }));
}

TEST(Maybe, doOnComplete)
{
	bool completed = false;
	EXPECT_NO_THROW(Maybe<int>::just(defaultValue)
						.doOnComplete(
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							})
						.subscribe());
	EXPECT_TRUE(completed);

	EXPECT_NO_THROW(Maybe<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.doOnComplete([]() { throw runtime_error("completion handler called"); })
						.subscribe());

	EXPECT_NO_THROW(Maybe<int>::never().doOnComplete([]() { throw runtime_error("completion handler called"); }).subscribe());

	completed = false;
	EXPECT_NO_THROW(Maybe<int>::empty()
						.doOnComplete(
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							})
						.subscribe());
	EXPECT_TRUE(completed);
}

TEST(Maybe, doOnError)
{
	bool errored = false;
	EXPECT_NO_THROW(Maybe<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.doOnError(
							[&errored](const auto &exception)
							{
								if (errored)
									throw runtime_error("error handler called twice");
								errored = true;
							})
						.subscribe());
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Maybe<int>::just(defaultValue).doOnError([](const auto &exception) { throw runtime_error("error handler called"); }).subscribe());

	EXPECT_NO_THROW(Maybe<int>::never().doOnError([](const auto &exception) { throw runtime_error("error handler called"); }).subscribe());

	EXPECT_NO_THROW(Maybe<int>::empty().doOnError([](const auto &exception) { throw runtime_error("error handler called"); }).subscribe());
}

TEST(Maybe, doOnNext)
{
	bool succeeded = false;
	EXPECT_NO_THROW(Maybe<int>::just(defaultValue)
						.doOnNext(
							[&succeeded](const auto value)
							{
								if (succeeded)
									throw runtime_error("success handler called twice");
								succeeded = true;
							})
						.subscribe());
	EXPECT_TRUE(succeeded);

	EXPECT_NO_THROW(Maybe<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.doOnNext([](const auto value) { throw runtime_error("success handler called"); })
						.subscribe());

	EXPECT_NO_THROW(Maybe<int>::never().doOnNext([](const auto value) { throw runtime_error("success handler called"); }).subscribe());

	EXPECT_NO_THROW(Maybe<int>::empty().doOnNext([](const auto value) { throw runtime_error("success handler called"); }).subscribe());
}

TEST(Maybe, doOnTerminate)
{
	bool terminated = false;
	EXPECT_NO_THROW(Maybe<int>::just(defaultValue)
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
	EXPECT_NO_THROW(Maybe<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.doOnTerminate(
							[&terminated]()
							{
								if (terminated)
									throw runtime_error("termination handler called twice");
								terminated = true;
							})
						.subscribe());
	EXPECT_TRUE(terminated);

	EXPECT_NO_THROW(Maybe<int>::never().doOnTerminate([]() { throw runtime_error("termination handler called"); }).subscribe());

	terminated = false;
	EXPECT_NO_THROW(Maybe<int>::empty()
						.doOnTerminate(
							[&terminated]()
							{
								if (terminated)
									throw runtime_error("termination handler called twice");
								terminated = true;
							})
						.subscribe());
	EXPECT_TRUE(terminated);
}

TEST(Maybe, tap)
{
	bool completed = false;
	bool succeeded = false;
	EXPECT_NO_THROW(Maybe<int>::just(defaultValue)
						.tap(
							[&succeeded](const auto value)
							{
								if (succeeded)
									throw runtime_error("success handler called twice");
								if (value != defaultValue)
									throw runtime_error("invalid value");
								succeeded = true;
							},
							[](const auto &exception) { throw runtime_error("error handler called"); },
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							})
						.subscribe());
	EXPECT_TRUE(completed);
	EXPECT_TRUE(succeeded);

	bool errored = false;
	EXPECT_NO_THROW(Maybe<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.tap([](const auto value) { throw runtime_error("success handler called"); },
							 [&errored](const auto &exception)
							 {
								 if (errored)
									 throw runtime_error("error handler called twice");
								 errored = true;
							 },
							 []() { throw runtime_error("completion handler called"); })
						.subscribe());
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Maybe<int>::never()
						.tap([](const auto value) { throw runtime_error("success handler called"); },
							 [](const auto &exception) { throw runtime_error("error handler called"); },
							 []() { throw runtime_error("completion handler called"); })
						.subscribe());

	completed = false;
	EXPECT_NO_THROW(Maybe<int>::empty()
						.tap([](const auto value) { throw runtime_error("success handler called"); },
							 [](const auto &exception) { throw runtime_error("error handler called"); },
							 [&completed]()
							 {
								 if (completed)
									 throw runtime_error("completion handler called twice");
								 completed = true;
							 })
						.subscribe());
	EXPECT_TRUE(completed);
}

TEST(Maybe, observeOn)
{
	WorkerThread worker;
	const auto	 workerThreadId = worker.threadId();
	const auto	 mainThreadId = this_thread::get_id();

	bool completed = false;
	bool testFailed = false;
	Maybe<int>::just(defaultValue)
		.doOnNext(
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
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(completed);
	EXPECT_FALSE(testFailed);

	bool errored = false;
	testFailed = false;
	Maybe<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
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
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(errored);
	EXPECT_FALSE(testFailed);
}

TEST(Maybe, subscribeOn)
{
	WorkerThread worker;
	const auto	 workerThreadId = worker.threadId();
	const auto	 mainThreadId = this_thread::get_id();

	bool completed = false;
	bool testFailed = false;
	Maybe<int>::just(defaultValue)
		.doOnNext(
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
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(completed);
	EXPECT_FALSE(testFailed);

	bool errored = false;
	testFailed = false;
	Maybe<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
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
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(errored);
	EXPECT_FALSE(testFailed);
}
