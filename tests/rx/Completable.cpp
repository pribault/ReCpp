#include <gtest/gtest.h>
#include <iostream>
#include <recpp/async/WorkerThread.h>
#include <recpp/rx/Completable.h>

using namespace recpp;
using namespace std;

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
		[](const auto &exception) { throw runtime_error("error handler called"); }));
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
							[](const auto &exception) { throw runtime_error("error handler called"); }));
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
						.subscribe([]() { throw runtime_error("completion handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   }));
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(
		Completable::create([](auto &subscriber) {})
			.subscribe([]() { throw runtime_error("completion handler called"); }, [](const auto &exception) { throw runtime_error("error handler called"); }));
}

TEST(Completable, defer)
{
	bool completed = false;
	EXPECT_NO_THROW(Completable::defer([]() { return Completable::complete(); })
						.subscribe(
							[&completed]()
							{
								if (completed)
									throw runtime_error("completion handler called twice");
								completed = true;
							},
							[](const auto &exception) { throw runtime_error("error handler called"); }));
	EXPECT_TRUE(completed);

	bool errored = false;
	EXPECT_NO_THROW(Completable::defer([]() { return Completable::error(make_exception_ptr(runtime_error("unexpected error!"))); })
						.subscribe([]() { throw runtime_error("completion handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   }));
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(
		Completable::defer([]() { return Completable::never(); })
			.subscribe([]() { throw runtime_error("completion handler called"); }, [](const auto &exception) { throw runtime_error("error handler called"); }));
}

TEST(Completable, error)
{
	bool errored = false;
	EXPECT_NO_THROW(Completable::error(make_exception_ptr(runtime_error("unexpected error!")))
						.subscribe([]() { throw runtime_error("completion handler called"); },
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
	EXPECT_NO_THROW(Completable::never().subscribe([]() { throw runtime_error("completion handler called"); },
												   [](const auto &exception) { throw runtime_error("error handler called"); }));
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
						.doOnComplete([]() { throw runtime_error("completion handler called"); })
						.subscribe());

	EXPECT_NO_THROW(Completable::never().doOnComplete([]() { throw runtime_error("completion handler called"); }).subscribe());
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

	EXPECT_NO_THROW(Completable::complete().doOnError([](const auto &exception) { throw runtime_error("error handler called"); }).subscribe());

	EXPECT_NO_THROW(Completable::never().doOnError([](const auto &exception) { throw runtime_error("error handler called"); }).subscribe());
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

	EXPECT_NO_THROW(Completable::never().doOnTerminate([]() { throw runtime_error("terminate handler called"); }).subscribe());
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
							[](const auto &exception) { throw runtime_error("error handler called"); })
						.subscribe());
	EXPECT_TRUE(completed);

	bool errored = false;
	EXPECT_NO_THROW(Completable::error(make_exception_ptr(runtime_error("unexpected error!")))
						.tap([]() { throw runtime_error("completion handler called"); },
							 [&errored](const auto &exception)
							 {
								 if (errored)
									 throw runtime_error("error handler called twice");
								 errored = true;
							 })
						.subscribe());
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(
		Completable::never()
			.tap([]() { throw runtime_error("completion handler called"); }, [](const auto &exception) { throw runtime_error("error handler called"); })
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
			[&testFailed](const auto &exception) { testFailed = true; });
	this_thread::sleep_for(chrono::milliseconds(2));
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
		.subscribe([&testFailed]() { testFailed = true; },
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
			[&testFailed](const auto &exception) { testFailed = true; });
	this_thread::sleep_for(chrono::milliseconds(2));
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
		.subscribe([&testFailed]() { testFailed = true; },
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

TEST(Completable, andThen)
{
	bool errored = false;
	EXPECT_NO_THROW(Completable::error(make_exception_ptr(runtime_error("unexpected error!")))
						.andThen(Completable::complete())
						.subscribe([]() { throw runtime_error("completion handler called"); },
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
						.subscribe([]() { throw runtime_error("completion handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   }));
	EXPECT_TRUE(errored);

	EXPECT_NO_THROW(
		Completable::never()
			.andThen(Completable::complete())
			.subscribe([]() { throw runtime_error("completion handler called"); }, [](const auto &exception) { throw runtime_error("error handler called"); }));

	EXPECT_NO_THROW(
		Completable::never()
			.andThen(Completable::error(make_exception_ptr(runtime_error("unexpected error!"))))
			.subscribe([]() { throw runtime_error("completion handler called"); }, [](const auto &exception) { throw runtime_error("error handler called"); }));

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
							[](const auto &exception) { throw runtime_error("error handler called"); }));
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
						.subscribe([]() { throw runtime_error("completion handler called"); },
								   [&errored](const auto &exception)
								   {
									   if (errored)
										   throw runtime_error("error handler called twice");
									   errored = true;
								   }));
	EXPECT_TRUE(deferCalled);
	EXPECT_TRUE(errored);

	deferCalled = false;
	EXPECT_NO_THROW(
		Completable::complete()
			.andThen(Completable::defer(
				[&deferCalled]()
				{
					if (deferCalled)
						throw runtime_error("defer handler called twice");
					deferCalled = true;
					return Completable::never();
				}))
			.subscribe([]() { throw runtime_error("completion handler called"); }, [](const auto &exception) { throw runtime_error("error handler called"); }));
	EXPECT_TRUE(deferCalled);
}
