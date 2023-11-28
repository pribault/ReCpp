// gtest
#include <gtest/gtest.h>

// recpp
#include <recpp/async/WorkerThread.h>
#include <recpp/rx/Observable.h>

// stl
#include <array>
#include <iostream>

using namespace recpp;
using namespace std;

namespace
{
	constexpr auto				 sleepDuration = chrono::milliseconds(10);
	constexpr int				 defaultValue = 42;
	constexpr std::array<int, 3> defaultValues({1, 2, 3});
} // namespace

TEST(Observable, create)
{
	size_t valuesCount = 0;
	bool   completed = false;
	EXPECT_NO_THROW(Observable<int>::create(
						[](auto &subscriber)
						{
							for (const auto value : defaultValues)
								subscriber.onNext(value);
							subscriber.onComplete();
							// Try to complete again, should not be forwared
							subscriber.onComplete();
							// Try to send an error, should not be forwared
							subscriber.onError(make_exception_ptr(runtime_error("unexpected error!")));
						})
						.subscribe(
							[&valuesCount](const auto value)
							{
								if (valuesCount >= defaultValues.size())
									throw runtime_error("too much values forwarded");
								const auto expectedValue = defaultValues[valuesCount++];
								if (value != expectedValue)
									throw runtime_error("unexpected value");
							},
							[](const auto &exception) { throw runtime_error("error handler called"); },
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							}));
	EXPECT_TRUE(completed);
	EXPECT_EQ(valuesCount, defaultValues.size());

	bool errored = false;
	EXPECT_NO_THROW(Observable<int>::create(
						[](auto &subscriber)
						{
							subscriber.onError(make_exception_ptr(runtime_error("unexpected error!")));
							// Try to emit another error, should not be forwarded
							subscriber.onError(make_exception_ptr(runtime_error("unexpected error!")));
							// Try to send value, should not be forwarded
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
	EXPECT_NO_THROW(Observable<int>::create([](auto &subscriber) { subscriber.onComplete(); })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [](const auto &exception) { throw runtime_error("error handler called"); },
								   [&completed]()
								   {
									   if (completed)
										   throw runtime_error("completion handler called twice");
									   completed = true;
								   }));
	EXPECT_TRUE(completed);

	EXPECT_NO_THROW(Observable<int>::create([](auto &subscriber) {})
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [](const auto &exception) { throw runtime_error("error handler called"); }));
}

TEST(Observable, defer)
{
	size_t valuesCount = 0;
	bool   completed = false;
	EXPECT_NO_THROW(Observable<int>::defer([]() { return Observable<int>::range(defaultValues); })
						.subscribe(
							[&valuesCount](const auto value)
							{
								if (valuesCount >= defaultValues.size())
									throw runtime_error("too much values forwarded");
								const auto expectedValue = defaultValues[valuesCount++];
								if (value != expectedValue)
									throw runtime_error("unexpected value");
							},
							[](const auto &exception) { throw runtime_error("error handler called"); },
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							}));
	EXPECT_TRUE(completed);
	EXPECT_EQ(valuesCount, defaultValues.size());

	bool errored = false;
	EXPECT_NO_THROW(Observable<int>::defer([]() { return Observable<int>::error(make_exception_ptr(runtime_error("unexpected error!"))); })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   },
								   []() { throw runtime_error("completion handler called"); }));
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Observable<int>::defer([]() { return Observable<int>::never(); })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [](const auto &exception) { throw runtime_error("error handler called"); },
								   []() { throw runtime_error("completion handler called"); }));

	completed = false;
	EXPECT_NO_THROW(Observable<int>::defer([]() { return Observable<int>::empty(); })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [](const auto &exception) { throw runtime_error("error handler called"); },
								   [&completed]()
								   {
									   if (completed)
										   throw runtime_error("completion handler called twice");
									   completed = true;
								   }));
	EXPECT_TRUE(completed);
}

TEST(Observable, empty)
{
	bool completed = false;
	EXPECT_NO_THROW(Observable<int>::empty().subscribe([](const auto value) { throw runtime_error("success handler called"); },
													   [](const auto &exception) { throw runtime_error("error handler called"); },
													   [&completed]()
													   {
														   if (completed)
															   throw runtime_error("completion handler called twice");
														   completed = true;
													   }));
	EXPECT_TRUE(completed);
}

TEST(Observable, error)
{
	bool errored = false;
	EXPECT_NO_THROW(Observable<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
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

TEST(Observable, just)
{
	bool succeeded = false;
	bool completed = false;
	EXPECT_NO_THROW(Observable<int>::just(defaultValue)
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

TEST(Observable, never)
{
	EXPECT_NO_THROW(Observable<int>::never().subscribe([](const auto value) { throw runtime_error("success handler called"); },
													   [](const auto &exception) { throw runtime_error("error handler called"); },
													   []() { throw runtime_error("completion handler called"); }));
}

TEST(Observable, range)
{
	size_t valuesCount = 0;
	bool   completed = false;
	EXPECT_NO_THROW(Observable<int>::defer([]() { return Observable<int>::range(defaultValues); })
						.subscribe(
							[&valuesCount](const auto value)
							{
								if (valuesCount >= defaultValues.size())
									throw runtime_error("too much values forwarded");
								const auto expectedValue = defaultValues[valuesCount++];
								if (value != expectedValue)
									throw runtime_error("unexpected value");
							},
							[](const auto &exception) { throw runtime_error("error handler called"); },
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							}));
	EXPECT_TRUE(completed);
	EXPECT_EQ(valuesCount, defaultValues.size());

	valuesCount = 0;
	completed = false;
	EXPECT_NO_THROW(Observable<int>::defer([]() { return Observable<int>::range(defaultValues.begin(), defaultValues.end()); })
						.subscribe(
							[&valuesCount](const auto value)
							{
								if (valuesCount >= defaultValues.size())
									throw runtime_error("too much values forwarded");
								const auto expectedValue = defaultValues[valuesCount++];
								if (value != expectedValue)
									throw runtime_error("unexpected value");
							},
							[](const auto &exception) { throw runtime_error("error handler called"); },
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							}));
	EXPECT_TRUE(completed);
	EXPECT_EQ(valuesCount, defaultValues.size());
}

TEST(Observable, filter)
{
	const std::vector<float> expectedValues({1, 3});
	size_t					 valuesCount = 0;
	bool					 completed = false;
	EXPECT_NO_THROW(Observable<int>::range(defaultValues)
						.filter([](const auto value) { return value % 2 == 1; })
						.subscribe(
							[&valuesCount, &expectedValues](const auto value)
							{
								if (valuesCount >= expectedValues.size())
									throw runtime_error("too much values forwarded");
								const auto expectedValue = expectedValues[valuesCount++];
								if (value != expectedValue)
									throw runtime_error("unexpected value");
							},
							[](const auto &exception) { throw runtime_error("error handler called"); },
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							}));
	EXPECT_TRUE(completed);
	EXPECT_EQ(valuesCount, expectedValues.size());

	bool errored = false;
	EXPECT_NO_THROW(Observable<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.filter([](const auto value) { return value % 2 == 1; })
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

TEST(Observable, map)
{
	size_t valuesCount = 0;
	bool   completed = false;
	EXPECT_NO_THROW(Observable<int>::range(defaultValues)
						.map<float>([](const auto value) { return static_cast<float>(value) / 1000; })
						.subscribe(
							[&valuesCount](const auto value)
							{
								if (valuesCount >= defaultValues.size())
									throw runtime_error("too much values forwarded");
								const auto expectedValue = static_cast<float>(defaultValues[valuesCount++]) / 1000;
								if (value != expectedValue)
									throw runtime_error("unexpected value");
							},
							[](const auto &exception) { throw runtime_error("error handler called"); },
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							}));
	EXPECT_TRUE(completed);
	EXPECT_EQ(valuesCount, defaultValues.size());

	bool errored = false;
	EXPECT_NO_THROW(Observable<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
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
	EXPECT_NO_THROW(Observable<int>::empty()
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

	EXPECT_NO_THROW(Observable<int>::never()
						.map<float>([](const auto value) { return static_cast<float>(value) / 1000; })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [](const auto &exception) { throw runtime_error("error handler called"); },
								   []() { throw runtime_error("completion handler called"); }));
}

TEST(Observable, flatMap)
{
	const std::vector<float> expectedValues({2, 3, 4, 3, 4, 5, 4, 5, 6});
	size_t					 valuesCount = 0;
	bool					 completed = false;
	EXPECT_NO_THROW(Observable<int>::range(defaultValues)
						.flatMap<float>(
							[](const auto value)
							{
								return Observable<float>::create(
									[value](auto &subscriber)
									{
										subscriber.onNext(value + 1.f);
										subscriber.onNext(value + 2.f);
										subscriber.onNext(value + 3.f);
										subscriber.onComplete();
									});
							})
						.subscribe(
							[&valuesCount, &expectedValues](const auto value)
							{
								if (valuesCount >= expectedValues.size())
									throw runtime_error("too much values forwarded");
								const auto expectedValue = expectedValues[valuesCount++];
								if (value != expectedValue)
									throw runtime_error("unexpected value");
							},
							[](const auto &exception) { throw runtime_error("error handler called"); },
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							}));
	EXPECT_TRUE(completed);
	EXPECT_EQ(valuesCount, expectedValues.size());

	bool errored = false;
	EXPECT_NO_THROW(Observable<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.flatMap<float>([](const auto value) { return Observable<float>::just(static_cast<float>(value) / 1000); })
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
	EXPECT_NO_THROW(Observable<int>::just(defaultValue)
						.flatMap<float>([](const auto value) { return Observable<float>::error(make_exception_ptr(runtime_error("unexpected error!"))); })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   },
								   []() { throw runtime_error("completion handler called"); }));
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Observable<int>::just(defaultValue)
						.flatMap<float>([](const auto value) { return Observable<float>::never(); })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [&errored](const auto &exception) { throw runtime_error("error handler called"); },
								   []() { throw runtime_error("completion handler called"); }));

	EXPECT_NO_THROW(Observable<int>::never()
						.flatMap<float>([](const auto value) { return Observable<float>::just(defaultValue); })
						.subscribe([](const auto value) { throw runtime_error("success handler called"); },
								   [&errored](const auto &exception) { throw runtime_error("error handler called"); },
								   []() { throw runtime_error("completion handler called"); }));
}

TEST(Observable, doOnComplete)
{
	bool completed = false;
	EXPECT_NO_THROW(Observable<int>::just(defaultValue)
						.doOnComplete(
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							})
						.subscribe());
	EXPECT_TRUE(completed);

	EXPECT_NO_THROW(Observable<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.doOnComplete([]() { throw runtime_error("completion handler called"); })
						.subscribe());

	EXPECT_NO_THROW(Observable<int>::never().doOnComplete([]() { throw runtime_error("completion handler called"); }).subscribe());

	completed = false;
	EXPECT_NO_THROW(Observable<int>::empty()
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

TEST(Observable, doOnError)
{
	bool errored = false;
	EXPECT_NO_THROW(Observable<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.doOnError(
							[&errored](const auto &exception)
							{
								if (errored)
									throw runtime_error("error handler called twice");
								errored = true;
							})
						.subscribe());
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(Observable<int>::just(defaultValue).doOnError([](const auto &exception) { throw runtime_error("error handler called"); }).subscribe());

	EXPECT_NO_THROW(Observable<int>::never().doOnError([](const auto &exception) { throw runtime_error("error handler called"); }).subscribe());

	EXPECT_NO_THROW(Observable<int>::empty().doOnError([](const auto &exception) { throw runtime_error("error handler called"); }).subscribe());
}

TEST(Observable, doOnNext)
{
	bool succeeded = false;
	EXPECT_NO_THROW(Observable<int>::just(defaultValue)
						.doOnNext(
							[&succeeded](const auto value)
							{
								if (succeeded)
									throw runtime_error("success handler called twice");
								succeeded = true;
							})
						.subscribe());
	EXPECT_TRUE(succeeded);

	EXPECT_NO_THROW(Observable<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.doOnNext([](const auto value) { throw runtime_error("success handler called"); })
						.subscribe());

	EXPECT_NO_THROW(Observable<int>::never().doOnNext([](const auto value) { throw runtime_error("success handler called"); }).subscribe());

	EXPECT_NO_THROW(Observable<int>::empty().doOnNext([](const auto value) { throw runtime_error("success handler called"); }).subscribe());
}

TEST(Observable, doOnTerminate)
{
	bool terminated = false;
	EXPECT_NO_THROW(Observable<int>::just(defaultValue)
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
	EXPECT_NO_THROW(Observable<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
						.doOnTerminate(
							[&terminated]()
							{
								if (terminated)
									throw runtime_error("termination handler called twice");
								terminated = true;
							})
						.subscribe());
	EXPECT_TRUE(terminated);

	EXPECT_NO_THROW(Observable<int>::never().doOnTerminate([]() { throw runtime_error("termination handler called"); }).subscribe());

	terminated = false;
	EXPECT_NO_THROW(Observable<int>::empty()
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

TEST(Observable, tap)
{
	bool completed = false;
	bool succeeded = false;
	EXPECT_NO_THROW(Observable<int>::just(defaultValue)
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
	EXPECT_NO_THROW(Observable<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
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

	EXPECT_NO_THROW(Observable<int>::never()
						.tap([](const auto value) { throw runtime_error("success handler called"); },
							 [](const auto &exception) { throw runtime_error("error handler called"); },
							 []() { throw runtime_error("completion handler called"); })
						.subscribe());

	completed = false;
	EXPECT_NO_THROW(Observable<int>::empty()
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

TEST(Observable, observeOn)
{
	WorkerThread worker;
	const auto	 workerThreadId = worker.threadId();
	const auto	 mainThreadId = this_thread::get_id();

	bool completed = false;
	bool testFailed = false;
	Observable<int>::just(defaultValue)
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
	Observable<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
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

TEST(Observable, subscribeOn)
{
	WorkerThread worker;
	const auto	 workerThreadId = worker.threadId();
	const auto	 mainThreadId = this_thread::get_id();

	bool completed = false;
	bool testFailed = false;
	Observable<int>::just(defaultValue)
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
	Observable<int>::error(make_exception_ptr(runtime_error("unexpected error!")))
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
