// fixtures
#include <fixtures/EventLoopBasedTest.h>
#include <fixtures/WorkerThreadBasedTest.h>

// gtest
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// recpp
#include <recpp/async/EventLoop.h>
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
	constexpr auto			sleepDuration = chrono::milliseconds(10);
	constexpr int			defaultValue = 42;
	constexpr array<int, 3> defaultValues({1, 2, 3});
	constexpr auto			delayTolerance = chrono::milliseconds(10);
	constexpr auto			delayDuration = chrono::milliseconds(100);
	constexpr auto			sleepDurationForDelay = chrono::milliseconds(150);
	constexpr string_view	runtimeErrorMessage = "unexpected error!";
} // namespace

class CompletableComplete : public testing::Test
{
};

TEST_F(CompletableComplete, CheckOnCompleteIsEmited)
{
	bool completed = false;
	Completable::complete() //
		.subscribe(
			[&completed]()
			{
				EXPECT_FALSE(completed);
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(CompletableComplete, CheckOnErrorIsNotEmited)
{
	Completable::complete() //
		.subscribe([]() {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

class CompletableCreate : public testing::Test
{
protected:
	static Completable complete()
	{
		return Completable::create(
			[](auto &subscriber)
			{
				subscriber.onComplete();
			});
	}
	static Completable completeTwice()
	{
		return Completable::create(
			[](auto &subscriber)
			{
				subscriber.onComplete();
				subscriber.onComplete();
			});
	}
	static Completable error()
	{
		return Completable::create(
			[](auto &subscriber)
			{
				subscriber.onError(make_exception_ptr(runtime_error(runtimeErrorMessage.data())));
			});
	}
	static Completable errorTwice()
	{
		return Completable::create(
			[](auto &subscriber)
			{
				subscriber.onError(make_exception_ptr(runtime_error(runtimeErrorMessage.data())));
				subscriber.onError(make_exception_ptr(runtime_error(runtimeErrorMessage.data())));
			});
	}
	static Completable errorAfterComplete()
	{
		return Completable::create(
			[](auto &subscriber)
			{
				subscriber.onComplete();
				subscriber.onError(make_exception_ptr(runtime_error(runtimeErrorMessage.data())));
			});
	}
	static Completable completeAfterError()
	{
		return Completable::create(
			[](auto &subscriber)
			{
				subscriber.onComplete();
				subscriber.onError(make_exception_ptr(runtime_error(runtimeErrorMessage.data())));
			});
	}
};

TEST_F(CompletableCreate, checkOnCompleteIsEmited)
{
	bool completed = false;
	complete() //
		.subscribe(
			[&completed]()
			{
				EXPECT_FALSE(completed);
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(CompletableCreate, checkOnErrorIsNotEmitedOnComplete)
{
	complete() //
		.subscribe([]() {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(CompletableCreate, checkOnCompleteIsNotEmitedTwice)
{
	bool completed = false;
	completeTwice() //
		.subscribe(
			[&completed]()
			{
				EXPECT_FALSE(completed);
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(CompletableCreate, checkOnErrorIsNotEmitedOnCompleteTwice)
{
	completeTwice() //
		.subscribe([]() {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(CompletableCreate, checkOnErrorIsEmited)
{
	bool gotError = false;
	error() //
		.subscribe([]() {},
				   [&gotError](const auto &exception)
				   {
					   EXPECT_FALSE(gotError);
					   gotError = true;
					   try
					   {
						   rethrow_exception(exception);
					   }
					   catch (runtime_error &runtimeError)
					   {
						   EXPECT_THAT(runtimeError.what(), testing::StrEq(runtimeErrorMessage));
					   }
				   });
	EXPECT_TRUE(gotError);
}

TEST_F(CompletableCreate, checkOnCompleteIsNotEmitedOnError)
{
	error() //
		.subscribe(
			[]()
			{
				ADD_FAILURE();
			});
}

TEST_F(CompletableCreate, checkOnErrorIsNotEmitedTwice)
{
	bool gotError = false;
	errorTwice() //
		.subscribe([]() {},
				   [&gotError](const auto &exception)
				   {
					   EXPECT_FALSE(gotError);
					   gotError = true;
					   try
					   {
						   rethrow_exception(exception);
					   }
					   catch (runtime_error &runtimeError)
					   {
						   EXPECT_THAT(runtimeError.what(), testing::StrEq(runtimeErrorMessage));
					   }
				   });
	EXPECT_TRUE(gotError);
}

TEST_F(CompletableCreate, checkOnErrorIsNotEmitedAfterComplete)
{
	errorAfterComplete() //
		.subscribe([]() {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(CompletableCreate, checkOnCompleteIsNotEmitedAfterError)
{
	completeAfterError() //
		.subscribe([]() {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

class CompletableDefer : public testing::Test
{
};

TEST_F(CompletableDefer, checkOnCompleteIsEmited)
{
	bool completed = false;
	Completable::defer(
		[]()
		{
			return Completable::complete();
		}) //
		.subscribe(
			[&completed]()
			{
				EXPECT_FALSE(completed);
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(CompletableDefer, checkOnErrorIsEmited)
{
	bool gotError = false;
	Completable::defer(
		[]()
		{
			return Completable::error(make_exception_ptr(runtime_error(runtimeErrorMessage.data())));
		}) //
		.subscribe([]() {},
				   [&gotError](const auto &exception)
				   {
					   EXPECT_FALSE(gotError);
					   gotError = true;
					   try
					   {
						   rethrow_exception(exception);
					   }
					   catch (runtime_error &runtimeError)
					   {
						   EXPECT_THAT(runtimeError.what(), testing::StrEq(runtimeErrorMessage));
					   }
				   });
	EXPECT_TRUE(gotError);
}

class CompletableError : public testing::Test
{
};

TEST_F(CompletableError, checkOnCompleteIsNotEmited)
{
	Completable::error(make_exception_ptr(runtime_error(runtimeErrorMessage.data()))) //
		.subscribe(
			[]()
			{
				ADD_FAILURE();
			});
}

TEST_F(CompletableError, checkOnErrorIsEmited)
{
	bool gotError = false;
	Completable::error(make_exception_ptr(runtime_error(runtimeErrorMessage.data()))) //
		.subscribe([]() {},
				   [&gotError](const auto &exception)
				   {
					   EXPECT_FALSE(gotError);
					   gotError = true;
					   try
					   {
						   rethrow_exception(exception);
					   }
					   catch (runtime_error &runtimeError)
					   {
						   EXPECT_THAT(runtimeError.what(), testing::StrEq(runtimeErrorMessage));
					   }
				   });
	EXPECT_TRUE(gotError);
}

class CompletableNever : public testing::Test
{
};

TEST_F(CompletableNever, checkNothingIsEmited)
{
	Completable::never().subscribe(
		[]()
		{
			ADD_FAILURE();
		},
		[](const auto &)
		{
			ADD_FAILURE();
		});
}

class CompletableMerge : public testing::Test
{
protected:
	static Completable complete()
	{
		Observable<Completable> completableList = Observable<Completable>::create(
			[](auto &subscriber)
			{
				subscriber.onNext(Completable::complete());
				subscriber.onNext(Completable::complete());
				subscriber.onNext(Completable::complete());
				subscriber.onComplete();
			});
		return Completable::merge(completableList);
	}
	static Completable error()
	{
		Observable<Completable> completableList = Observable<Completable>::create(
			[](auto &subscriber)
			{
				subscriber.onNext(Completable::error(make_exception_ptr(runtime_error(runtimeErrorMessage.data()))));
				subscriber.onComplete();
			});
		return Completable::merge(completableList);
	}
};

TEST_F(CompletableMerge, checkOnCompleteIsEmited)
{
	bool completed = false;
	complete() //
		.subscribe(
			[&completed]()
			{
				EXPECT_FALSE(completed);
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(CompletableMerge, checkOnErrorIsNotEmitedOnComplete)
{
	complete() //
		.subscribe([]() {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(CompletableMerge, checkOnErrorIsEmited)
{
	bool gotError = false;
	error() //
		.subscribe([]() {},
				   [&gotError](const auto &exception)
				   {
					   EXPECT_FALSE(gotError);
					   gotError = true;
					   try
					   {
						   rethrow_exception(exception);
					   }
					   catch (runtime_error &runtimeError)
					   {
						   EXPECT_THAT(runtimeError.what(), testing::StrEq(runtimeErrorMessage));
					   }
				   });
	EXPECT_TRUE(gotError);
}

TEST_F(CompletableMerge, checkOnCompleteIsNotEmitedOnError)
{
	error() //
		.subscribe(
			[]()
			{
				ADD_FAILURE();
			});
}

class CompletableDoOnComplete : public testing::Test
{
};

TEST_F(CompletableDoOnComplete, checkOnCompleteIsEmited)
{
	bool completed = false;
	Completable::complete() //
		.doOnComplete(
			[&completed]()
			{
				EXPECT_FALSE(completed);
				completed = true;
			})
		.subscribe();
	EXPECT_TRUE(completed);
}

TEST_F(CompletableDoOnComplete, checkOnCompleteIsNotEmitedOnError)
{
	Completable::error(make_exception_ptr(runtime_error(runtimeErrorMessage.data()))) //
		.doOnComplete(
			[]()
			{
				ADD_FAILURE();
			})
		.subscribe();
}

class CompletableDoOnError : public testing::Test
{
};

TEST_F(CompletableDoOnError, checkOnErrorIsEmited)
{
	bool gotError = false;
	Completable::error(make_exception_ptr(runtime_error(runtimeErrorMessage.data()))) //
		.doOnError(
			[&gotError](const auto &exception)
			{
				EXPECT_FALSE(gotError);
				gotError = true;
				try
				{
					rethrow_exception(exception);
				}
				catch (runtime_error &runtimeError)
				{
					EXPECT_THAT(runtimeError.what(), testing::StrEq(runtimeErrorMessage));
				}
			})
		.subscribe();
	EXPECT_TRUE(gotError);
}

TEST_F(CompletableDoOnError, checkOnErrorIsNotEmitedOnComplete)
{
	Completable::complete() //
		.doOnError(
			[](const auto &)
			{
				ADD_FAILURE();
			})
		.subscribe();
}

class CompletableDoOnTerminate : public testing::Test
{
};

TEST_F(CompletableDoOnTerminate, checkOnTerminateIsEmitedOnComplete)
{
	bool terminated = false;
	Completable::complete() //
		.doOnTerminate(
			[&terminated]()
			{
				EXPECT_FALSE(terminated);
				terminated = true;
			})
		.subscribe();
	EXPECT_TRUE(terminated);
}

TEST_F(CompletableDoOnTerminate, checkOnTerminateIsEmitedOnError)
{
	bool terminated = false;
	Completable::error(make_exception_ptr(runtime_error(runtimeErrorMessage.data()))) //
		.doOnTerminate(
			[&terminated]()
			{
				EXPECT_FALSE(terminated);
				terminated = true;
			})
		.subscribe();
	EXPECT_TRUE(terminated);
}

class CompletableTap : public testing::Test
{
};

TEST_F(CompletableTap, checkOnCompleteIsEmited)
{
	bool completed = false;
	Completable::complete() //
		.tap(
			[&completed]()
			{
				EXPECT_FALSE(completed);
				completed = true;
			},
			[](const auto &) {})
		.subscribe();
	EXPECT_TRUE(completed);
}

TEST_F(CompletableTap, checkOnCompleteIsNotEmitedOnError)
{
	Completable::error(make_exception_ptr(runtime_error(runtimeErrorMessage.data()))) //
		.tap(
			[]()
			{
				ADD_FAILURE();
			},
			[](const auto &) {})
		.subscribe();
}

TEST_F(CompletableTap, checkOnErrorIsEmited)
{
	bool gotError = false;
	Completable::error(make_exception_ptr(runtime_error(runtimeErrorMessage.data()))) //
		.tap([]() {},
			 [&gotError](const auto &exception)
			 {
				 EXPECT_FALSE(gotError);
				 gotError = true;
				 try
				 {
					 rethrow_exception(exception);
				 }
				 catch (runtime_error &runtimeError)
				 {
					 EXPECT_THAT(runtimeError.what(), testing::StrEq(runtimeErrorMessage));
				 }
			 })
		.subscribe();
	EXPECT_TRUE(gotError);
}

TEST_F(CompletableTap, checkOnErrorIsNotEmitedOnComplete)
{
	Completable::complete() //
		.tap([]() {},
			 [](const auto &)
			 {
				 ADD_FAILURE();
			 })
		.subscribe();
}

class CompletableObserveOn : public WorkerThreadBasedTest
{
};

TEST_F(CompletableObserveOn, checkSubscribeOnMainThread)
{
	bool deferCalled = false;
	Completable::defer(
		[this, &deferCalled]()
		{
			EXPECT_EQ(this_thread::get_id(), m_mainThreadId);
			deferCalled = true;
			return Completable::complete();
		}) //
		.observeOn(*m_worker)
		.subscribe();
	EXPECT_TRUE(deferCalled);
}

TEST_F(CompletableObserveOn, checkDoOnCompleteCalledOnWorkerThread)
{
	bool completed = false;
	Completable::complete() //
		.observeOn(*m_worker)
		.doOnComplete(
			[this, &completed]()
			{
				EXPECT_EQ(this_thread::get_id(), m_workerThreadId);
				completed = true;
			})
		.subscribe();
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(completed);
}

TEST_F(CompletableObserveOn, checkDoOnErrorCalledOnWorkerThread)
{
	bool gotError = false;
	Completable::error(make_exception_ptr(runtime_error(runtimeErrorMessage.data()))) //
		.observeOn(*m_worker)
		.doOnError(
			[this, &gotError](const auto &exception)
			{
				EXPECT_EQ(this_thread::get_id(), m_workerThreadId);
				gotError = true;
				try
				{
					rethrow_exception(exception);
				}
				catch (runtime_error &runtimeError)
				{
					EXPECT_THAT(runtimeError.what(), testing::StrEq(runtimeErrorMessage));
				}
			})
		.subscribe();
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(gotError);
}

class CompletableSubscribeOn : public WorkerThreadBasedTest
{
};

TEST_F(CompletableSubscribeOn, checkSubscribeOnWorkerThread)
{
	bool deferCalled = false;
	Completable::defer(
		[this, &deferCalled]()
		{
			EXPECT_EQ(this_thread::get_id(), m_workerThreadId);
			deferCalled = true;
			return Completable::complete();
		}) //
		.subscribeOn(*m_worker)
		.subscribe();
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(deferCalled);
}

TEST_F(CompletableSubscribeOn, checkDoOnCompleteCalledOnWorkerThread)
{
	bool completed = false;
	Completable::complete() //
		.subscribeOn(*m_worker)
		.doOnComplete(
			[this, &completed]()
			{
				EXPECT_EQ(this_thread::get_id(), m_workerThreadId);
				completed = true;
			})
		.subscribe();
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(completed);
}

TEST_F(CompletableSubscribeOn, checkDoOnErrorCalledOnWorkerThread)
{
	bool gotError = false;
	Completable::error(make_exception_ptr(runtime_error(runtimeErrorMessage.data()))) //
		.subscribeOn(*m_worker)
		.doOnError(
			[this, &gotError](const auto &exception)
			{
				EXPECT_EQ(this_thread::get_id(), m_workerThreadId);
				gotError = true;
				try
				{
					rethrow_exception(exception);
				}
				catch (runtime_error &runtimeError)
				{
					EXPECT_THAT(runtimeError.what(), testing::StrEq(runtimeErrorMessage));
				}
			})
		.subscribe();
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(gotError);
}

class CompletableAndThen : public testing::Test
{
};

TEST_F(CompletableAndThen, checkNextCompletableIsCalledOnComplete)
{
	bool deferCalled = false;
	Completable::complete() //
		.andThen(Completable::defer(
			[&deferCalled]()
			{
				EXPECT_FALSE(deferCalled);
				deferCalled = true;
				return Completable::complete();
			}))
		.subscribe();
	EXPECT_TRUE(deferCalled);
}

TEST_F(CompletableAndThen, checkNextSingleIsCalledOnComplete)
{
	bool deferCalled = false;
	Completable::complete() //
		.andThen(Single<int>::defer(
			[&deferCalled]()
			{
				EXPECT_FALSE(deferCalled);
				deferCalled = true;
				return Single<int>::just(defaultValue);
			}))
		.subscribe();
	EXPECT_TRUE(deferCalled);
}

TEST_F(CompletableAndThen, checkNextMaybeIsCalledOnComplete)
{
	bool deferCalled = false;
	Completable::complete() //
		.andThen(Maybe<int>::defer(
			[&deferCalled]()
			{
				EXPECT_FALSE(deferCalled);
				deferCalled = true;
				return Maybe<int>::empty();
			}))
		.subscribe();
	EXPECT_TRUE(deferCalled);
}

TEST_F(CompletableAndThen, checkNextObservableIsCalledOnComplete)
{
	bool deferCalled = false;
	Completable::complete() //
		.andThen(Observable<int>::defer(
			[&deferCalled]()
			{
				EXPECT_FALSE(deferCalled);
				deferCalled = true;
				return Observable<int>::empty();
			}))
		.subscribe();
	EXPECT_TRUE(deferCalled);
}

TEST_F(CompletableAndThen, checkAndThenIsNotCalledOnError)
{
	bool deferCalled = false;
	Completable::error(make_exception_ptr(runtime_error(runtimeErrorMessage.data()))) //
		.andThen(Completable::defer(
			[&deferCalled]()
			{
				deferCalled = true;
				return Completable::complete();
			}))
		.subscribe();
	EXPECT_FALSE(deferCalled);
}

class CompletableDelay : public EventLoopBasedTest
{
};

TEST_F(CompletableDelay, checkOnCompleteDelay)
{
	recpp::async::Scheduler::TimePoint start;
	recpp::async::Scheduler::TimePoint end;
	Completable::defer(
		[&start]()
		{
			start = recpp::async::Scheduler::Clock::now();
			return Completable::complete();
		}) //
		.delay(*m_eventLoop, delayDuration, true)
		.subscribe(
			[this, &end]()
			{
				end = recpp::async::Scheduler::Clock::now();
				m_eventLoop->stop();
			},
			[](const auto &) {});

	m_eventLoop->run();
	auto timeDiff = end - start;
	auto gap = timeDiff - delayDuration;
	EXPECT_LT(chrono::abs(gap), delayTolerance);
}

TEST_F(CompletableDelay, checkOnErrorDelay)
{
	recpp::async::Scheduler::TimePoint start;
	recpp::async::Scheduler::TimePoint end;
	Completable::defer(
		[&start]()
		{
			start = recpp::async::Scheduler::Clock::now();
			return Completable::error(make_exception_ptr(runtime_error(runtimeErrorMessage.data())));
		}) //
		.delay(*m_eventLoop, delayDuration, true)
		.subscribe([]() {},
				   [this, &end](const auto &)
				   {
					   end = recpp::async::Scheduler::Clock::now();
					   m_eventLoop->stop();
				   });

	m_eventLoop->run();
	auto timeDiff = end - start;
	auto gap = timeDiff - delayDuration;
	EXPECT_LT(chrono::abs(gap), delayTolerance);
}

TEST_F(CompletableDelay, checkNoErrorDelayMode)
{
	recpp::async::Scheduler::TimePoint start;
	recpp::async::Scheduler::TimePoint end;
	Completable::defer(
		[&start]()
		{
			start = recpp::async::Scheduler::Clock::now();
			return Completable::error(make_exception_ptr(runtime_error(runtimeErrorMessage.data())));
		}) //
		.delay(*m_eventLoop, delayDuration, false)
		.subscribe([]() {},
				   [this, &end](const auto &)
				   {
					   end = recpp::async::Scheduler::Clock::now();
					   m_eventLoop->stop();
				   });

	m_eventLoop->run();
	auto timeDiff = end - start;
	EXPECT_LT(chrono::abs(timeDiff), delayTolerance);
}
