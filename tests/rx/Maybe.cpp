// fixtures
#include <fixtures/EventLoopBasedTest.h>
#include <fixtures/WorkerThreadBasedTest.h>

// gtest
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// recpp
#include <recpp/async/EventLoop.h>
#include <recpp/async/WorkerThread.h>
#include <recpp/rx/Maybe.h>

// stl
#include <iostream>

using namespace recpp::async;
using namespace recpp::rx;
using namespace std;

namespace
{
	constexpr int		  defaultValue = 42;
	constexpr int		  otherValue = 55;
	const vector<int>	  defaultValues = {1, 2, 3};
	constexpr auto		  sleepDuration = chrono::milliseconds(10);
	constexpr auto		  delayTolerance = chrono::milliseconds(10);
	constexpr auto		  delayDuration = chrono::milliseconds(100);
	constexpr auto		  sleepDurationForDelay = chrono::milliseconds(150);
	constexpr string_view runtimeErrorMessage = "unexpected error!";

	float divideByTen(int value)
	{
		return value / 10.f;
	}
} // namespace

class MaybeCreate : public testing::Test
{
protected:
	static Maybe<int> listOfValues()
	{
		return Maybe<int>::create(
			[](auto &subscriber)
			{
				for (const auto value : defaultValues)
					subscriber.onNext(value);
				subscriber.onComplete();
			});
	}
	static Maybe<int> completesTwice()
	{
		return Maybe<int>::create(
			[](auto &subscriber)
			{
				for (const auto value : defaultValues)
					subscriber.onNext(value);
				subscriber.onComplete();
				subscriber.onComplete();
			});
	}
	static Maybe<int> valueAfterCompletion()
	{
		return Maybe<int>::create(
			[](auto &subscriber)
			{
				subscriber.onComplete();
				subscriber.onNext(42);
			});
	}
	static Maybe<int> errored()
	{
		return Maybe<int>::create(
			[](auto &subscriber)
			{
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
			});
	}
	static Maybe<int> valueAfterError()
	{
		return Maybe<int>::create(
			[](auto &subscriber)
			{
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
				subscriber.onNext(42);
			});
	}
	static Maybe<int> doubleError()
	{
		return Maybe<int>::create(
			[](auto &subscriber)
			{
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
			});
	}
	static Maybe<int> completeAfterError()
	{
		return Maybe<int>::create(
			[](auto &subscriber)
			{
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
				subscriber.onComplete();
			});
	}
};

TEST_F(MaybeCreate, checkEmitedValues)
{
	vector<int> result;
	bool		completed = false;
	listOfValues() //
		.subscribe(
			[&result](const auto value)
			{
				EXPECT_EQ(value, defaultValues[0]);
				result.push_back(value);
			},
			[](const auto &) {},
			[&completed, &result]()
			{
				completed = true;
				EXPECT_EQ(result.size(), 1);
			});
	EXPECT_TRUE(completed);
}

TEST_F(MaybeCreate, checkOnCompleteIsEmited)
{
	bool completed = false;
	listOfValues() //
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&completed]()
				   {
					   EXPECT_FALSE(completed);
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(MaybeCreate, checkOnCompleteIsNotEmitedTwice)
{
	bool completed = false;
	completesTwice() //
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&completed]()
				   {
					   EXPECT_FALSE(completed);
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(MaybeCreate, checkValuesAfterCompleteArentEmited)
{
	valueAfterCompletion() //
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

TEST_F(MaybeCreate, checkEmitedError)
{
	bool gotError = false;
	errored() //
		.subscribe([](const auto) {},
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

TEST_F(MaybeCreate, checkValuesAfterErrorArentEmited)
{
	valueAfterError() //
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

TEST_F(MaybeCreate, checkCompleteAfterErrorIsntEmited)
{
	completeAfterError() //
		.subscribe([](const auto) {}, [](const auto &) {},
				   []()
				   {
					   ADD_FAILURE();
				   });
}

class MaybeDefer : public testing::Test
{
protected:
	static Maybe<int> deferredValue()
	{
		return Maybe<int>::defer(
			[]()
			{
				return Maybe<int>::just(defaultValue);
			});
	}
	static Maybe<int> deferredEmpty()
	{
		return Maybe<int>::defer(
			[]()
			{
				return Maybe<int>::empty();
			});
	}
	static Maybe<int> deferredError()
	{
		return Maybe<int>::defer(
			[]()
			{
				return Maybe<int>::error(runtime_error(runtimeErrorMessage.data()));
			});
	}
};

TEST_F(MaybeDefer, checkEmitedValue)
{
	vector<int> result;
	bool		completed = false;
	deferredValue() //
		.subscribe(
			[&result](const auto value)
			{
				EXPECT_EQ(value, defaultValue);
				result.push_back(value);
			},
			[](const auto &) {},
			[&completed, &result]()
			{
				completed = true;
				EXPECT_EQ(result.size(), 1);
			});
	EXPECT_TRUE(completed);
}

TEST_F(MaybeDefer, checkOnCompleteIsEmited)
{
	bool completed = false;
	deferredEmpty() //
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&completed]()
				   {
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(MaybeDefer, checkEmitedError)
{
	bool gotError = false;
	deferredError() //
		.subscribe([](const auto) {},
				   [&gotError](const auto &exception)
				   {
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

class MaybeEmpty : public testing::Test
{
};

TEST_F(MaybeEmpty, checkOnCompleteIsEmited)
{
	bool completed = false;
	Maybe<int>::empty() //
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&completed]()
				   {
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(MaybeEmpty, checkOnCompleteIsNotCalledTwice)
{
	bool completed = false;
	Maybe<int>::empty() //
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&completed]()
				   {
					   EXPECT_FALSE(completed);
					   completed = true;
				   });
}

TEST_F(MaybeEmpty, checkNoValueIsEmited)
{
	Maybe<int>::empty() //
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

TEST_F(MaybeEmpty, checkNoErrorIsEmited)
{
	Maybe<int>::empty() //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

class MaybeError : public testing::Test
{
protected:
	static Maybe<int> errored()
	{
		return Maybe<int>::error(runtime_error(runtimeErrorMessage.data()));
	}
};

TEST_F(MaybeError, checkOnErrorIsEmited)
{
	bool gotError = false;
	errored() //
		.subscribe([](const auto) {},
				   [&gotError](const auto &exception)
				   {
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

TEST_F(MaybeError, checkOnlyOneErrorIsEmited)
{
	bool gotError = false;
	errored() //
		.subscribe([](const auto) {},
				   [&gotError](const auto &exception)
				   {
					   EXPECT_FALSE(gotError);
					   gotError = true;
				   });
	EXPECT_TRUE(gotError);
}

TEST_F(MaybeError, checkNoValueIsEmited)
{
	errored() //
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

TEST_F(MaybeError, checkOnCompleteIsntEmited)
{
	errored() //
		.subscribe([](const auto) {}, [](const auto &) {},
				   []()
				   {
					   ADD_FAILURE();
				   });
}

class MaybeJust : public testing::Test
{
};

TEST_F(MaybeJust, checkValueIsEmitted)
{
	bool gotValue = false;
	Maybe<int>::just(defaultValue)
		.subscribe(
			[&gotValue](const auto value)
			{
				EXPECT_EQ(value, defaultValue);
				gotValue = true;
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(MaybeJust, checkOnlyOneValueIsEmitted)
{
	bool gotValue = false;
	Maybe<int>::just(defaultValue)
		.subscribe(
			[&gotValue](const auto value)
			{
				EXPECT_FALSE(gotValue);
				gotValue = true;
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(MaybeJust, checkOnCompleteIsEmited)
{
	bool completed = false;
	Maybe<int>::just(defaultValue)
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&completed]()
				   {
					   EXPECT_FALSE(completed);
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(MaybeJust, checkNoError)
{
	Maybe<int>::just(defaultValue)
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

class MaybeNever : public testing::Test
{
};

TEST_F(MaybeNever, checkNothingIsEmited)
{
	Maybe<int>::never().subscribe(
		[](const auto)
		{
			ADD_FAILURE();
		},
		[](const auto &)
		{
			ADD_FAILURE();
		},
		[]()
		{
			ADD_FAILURE();
		});
}

class MaybeMap : public testing::Test
{
};

TEST_F(MaybeMap, checkEmitedValues)
{
	Maybe<int>::just(defaultValue) //
		.template map<float>(&divideByTen)
		.subscribe(
			[](const auto value)
			{
				EXPECT_EQ(value, divideByTen(defaultValue));
			});
}

TEST_F(MaybeMap, checkNoErrorIsEmited)
{
	Maybe<int>::just(defaultValue) //
		.template map<float>(&divideByTen)
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(MaybeMap, checkErrorsAreForwarded)
{
	bool gotError = false;
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.template map<float>(&divideByTen)
		.subscribe([](const auto) {},
				   [&gotError](const auto &exception)
				   {
					   gotError = true;
					   try
					   {
						   rethrow_exception(exception);
					   }
					   catch (runtime_error &runtimeError)
					   {
						   EXPECT_THAT(runtimeError.what(), testing::StrEq(runtimeErrorMessage));
					   }
				   },
				   []() {});
	EXPECT_TRUE(gotError);
}

TEST_F(MaybeMap, checkDoNotCompleteOnError)
{
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.template map<float>(&divideByTen)
		.subscribe([](const auto) {}, [](const auto &) {},
				   []()
				   {
					   ADD_FAILURE();
				   });
}

class MaybeFlatMap : public testing::Test
{
protected:
	static Maybe<float> transformedValue()
	{
		return Maybe<int>::just(defaultValue) //
			.flatMap<float>(
				[](const auto value)
				{
					return Maybe<float>::just(divideByTen(value));
				});
	}
};

TEST_F(MaybeFlatMap, checkEmitedValue)
{
	transformedValue() //
		.subscribe(
			[](const auto value)
			{
				EXPECT_EQ(value, divideByTen(defaultValue));
			});
}

TEST_F(MaybeFlatMap, checkEmitedCompletes)
{
	bool completed = false;
	transformedValue() //
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&completed]()
				   {
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(MaybeFlatMap, checkNoErrorIsEmited)
{
	transformedValue() //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(MaybeFlatMap, checkErrorsAreForwarded)
{
	bool gotError = false;
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.flatMap<float>(
			[](const auto)
			{
				return Maybe<float>::just(defaultValue);
			})
		.subscribe([](const auto) {},
				   [&gotError](const auto &exception)
				   {
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

TEST_F(MaybeFlatMap, checkDoNotCompleteOnError)
{
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.flatMap<float>(
			[](const auto)
			{
				return Maybe<float>::just(defaultValue);
			})
		.subscribe([](const auto) {}, [](const auto &) {},
				   []()
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(MaybeFlatMap, checkCanEmitErrors)
{
	bool gotError = false;
	Maybe<int>::just(defaultValue) //
		.flatMap<float>(
			[](const auto)
			{
				return Maybe<float>::error(runtime_error(runtimeErrorMessage.data()));
			})
		.subscribe([](const auto) {},
				   [&gotError](const auto &exception)
				   {
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

class MaybeMerge : public testing::Test
{
protected:
	static Observable<Maybe<int>> observableOfMaybe()
	{
		return Observable<Maybe<int>>::create(
			[](auto &subscriber)
			{
				subscriber.onNext(Maybe<int>::just(defaultValues[0]));
				subscriber.onNext(Maybe<int>::empty());
				subscriber.onNext(Maybe<int>::just(defaultValues[1]));
				subscriber.onNext(Maybe<int>::just(defaultValues[2]));
				subscriber.onComplete();
			});
	}
	static Observable<Maybe<int>> observableOfMaybeWithError()
	{
		return Observable<Maybe<int>>::create(
			[](auto &subscriber)
			{
				subscriber.onNext(Maybe<int>::just(defaultValues[0]));
				subscriber.onNext(Maybe<int>::error(runtime_error(runtimeErrorMessage.data())));
				subscriber.onNext(Maybe<int>::just(defaultValues[2]));
				subscriber.onComplete();
			});
	}
};

TEST_F(MaybeMerge, checkEmitedValues)
{
	vector<int> result;
	bool		completed = false;
	auto		source = observableOfMaybe();
	Maybe<int>::merge(source) //
		.subscribe(
			[&result](const auto value)
			{
				result.push_back(value);
			},
			[](const auto &) {},
			[&completed, &result]()
			{
				completed = true;
				EXPECT_EQ(result, defaultValues);
			});
	EXPECT_TRUE(completed);
}

TEST_F(MaybeMerge, checkOnCompleteIsEmited)
{
	bool completed = false;
	auto source = observableOfMaybe();
	Maybe<int>::merge(source) //
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&completed]()
				   {
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(MaybeMerge, checkNoErrorIsEmited)
{
	auto source = observableOfMaybe();
	Maybe<int>::merge(source) //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(MaybeMerge, checkErrorCanBeEmited)
{
	bool gotError = false;
	auto source = observableOfMaybeWithError();
	Maybe<int>::merge(source) //
		.subscribe([](const auto) {},
				   [&gotError](const auto &exception)
				   {
					   gotError = true;
					   try
					   {
						   rethrow_exception(exception);
					   }
					   catch (runtime_error &runtimeError)
					   {
						   EXPECT_THAT(runtimeError.what(), testing::StrEq(runtimeErrorMessage));
					   }
				   },
				   []()
				   {
					   ADD_FAILURE();
				   });
	EXPECT_TRUE(gotError);
}

class MaybeIgnoreElement : public testing::Test
{
};

TEST_F(MaybeIgnoreElement, checkCompletes)
{
	bool completed = false;
	Maybe<int>::just(defaultValue) //
		.ignoreElement()
		.subscribe(
			[&completed]()
			{
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(MaybeIgnoreElement, checkNoErrorIsEmited)
{
	Maybe<int>::just(defaultValue) //
		.ignoreElement()
		.subscribe([]() {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(MaybeIgnoreElement, checkErrorsAreForwarded)
{
	bool gotError = false;
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.ignoreElement()
		.subscribe([]() {},
				   [&gotError](const auto &exception)
				   {
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

TEST_F(MaybeIgnoreElement, checkDoNotCompleteOnError)
{
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.ignoreElement()
		.subscribe(
			[]()
			{
				ADD_FAILURE();
			});
}

class MaybeDoOnComplete : public testing::Test
{
};

TEST_F(MaybeDoOnComplete, checkDoOnCompleteIsCalledOnce)
{
	bool completed = false;
	Maybe<int>::just(defaultValue) //
		.doOnComplete(
			[&completed]()
			{
				EXPECT_FALSE(completed);
				completed = true;
			})
		.subscribe();
	EXPECT_TRUE(completed);
}

TEST_F(MaybeDoOnComplete, checkDoOnCompleteIsNotCalledOnError)
{
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.doOnComplete(
			[]()
			{
				ADD_FAILURE();
			})
		.subscribe();
}

class MaybeDoOnError : public testing::Test
{
};

TEST_F(MaybeDoOnError, checkDoOnErrorIsNotCalledWhenNoErrorIsEmited)
{
	Maybe<int>::just(defaultValue) //
		.doOnError(
			[](const auto &)
			{
				ADD_FAILURE();
			})
		.subscribe();
}

TEST_F(MaybeDoOnError, checkDoOnErrorIsCalledOnError)
{
	bool gotError = false;
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.doOnError(
			[&gotError](const auto &exception)
			{
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

class MaybeDoOnNext : public testing::Test
{
};

TEST_F(MaybeDoOnNext, checkDoOnNextIsCalledForEachValue)
{
	Maybe<int>::just(defaultValue) //
		.doOnNext(
			[](const auto value)
			{
				EXPECT_EQ(value, defaultValue);
			})
		.subscribe();
}

TEST_F(MaybeDoOnNext, checkDoOnNextIsNotCalledInCaseOfAnError)
{
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.doOnNext(
			[](const auto value)
			{
				ADD_FAILURE();
			})
		.subscribe();
}

class MaybeDoOnTerminate : public testing::Test
{
};

TEST_F(MaybeDoOnTerminate, checkDoOnTerminateIsCalledAfterComplete)
{
	bool terminated = false;
	Maybe<int>::just(defaultValue) //
		.doOnTerminate(
			[&terminated]()
			{
				terminated = true;
			})
		.subscribe();
	EXPECT_TRUE(terminated);
}

TEST_F(MaybeDoOnTerminate, checkDoOnTerminateIsCalledOnError)
{
	bool terminated = false;
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.doOnTerminate(
			[&terminated]()
			{
				terminated = true;
			})
		.subscribe();
	EXPECT_TRUE(terminated);
}

class MaybeTap : public testing::Test
{
};

TEST_F(MaybeTap, checkTapDoOnCompleteIsCalledOnce)
{
	bool completed = false;
	Maybe<int>::just(defaultValue) //
		.tap([](const auto) {}, [](const auto &) {},
			 [&completed]()
			 {
				 EXPECT_FALSE(completed);
				 completed = true;
			 })
		.subscribe();
	EXPECT_TRUE(completed);
}

TEST_F(MaybeTap, checkTapDoOnCompleteIsNotCalledOnError)
{
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.tap([](const auto) {}, [](const auto &) {},
			 []()
			 {
				 ADD_FAILURE();
			 })
		.subscribe();
}

TEST_F(MaybeTap, checkTapDoOnErrorIsNotCalledWhenNoErrorIsEmited)
{
	Maybe<int>::just(defaultValue) //
		.tap([](const auto) {},
			 [](const auto &)
			 {
				 ADD_FAILURE();
			 },
			 []() {})
		.subscribe();
}

TEST_F(MaybeTap, checkTapDoOnErrorIsCalledOnError)
{
	bool gotError = false;
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.tap([](const auto) {},
			 [&gotError](const auto &exception)
			 {
				 gotError = true;
				 try
				 {
					 rethrow_exception(exception);
				 }
				 catch (runtime_error &runtimeError)
				 {
					 EXPECT_THAT(runtimeError.what(), testing::StrEq(runtimeErrorMessage));
				 }
			 },
			 []() {})
		.subscribe();
	EXPECT_TRUE(gotError);
}

TEST_F(MaybeTap, checkTapDoOnNextIsCalled)
{
	Maybe<int>::just(defaultValue) //
		.tap(
			[](const auto value)
			{
				EXPECT_EQ(value, defaultValue);
			},
			[](const auto &) {}, []() {})
		.subscribe();
}

TEST_F(MaybeTap, checkTapDoOnNextIsNotCalledInCaseOfAnError)
{
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.tap(
			[](const auto value)
			{
				ADD_FAILURE();
			},
			[](const auto &) {}, []() {})
		.subscribe();
}

class MaybeObserveOn : public WorkerThreadBasedTest
{
};

TEST_F(MaybeObserveOn, checkSubscribeOnMainThread)
{
	bool deferCalled = false;
	Maybe<int>::defer(
		[this, &deferCalled]()
		{
			EXPECT_EQ(this_thread::get_id(), m_mainThreadId);
			deferCalled = true;
			return Maybe<int>::empty();
		}) //
		.observeOn(*m_worker)
		.subscribe();
	EXPECT_TRUE(deferCalled);
}

TEST_F(MaybeObserveOn, checkDoOnNextCalledOnWorkerThread)
{
	bool doOnNextCalled = false;
	Maybe<int>::just(defaultValue) //
		.observeOn(*m_worker)
		.doOnNext(
			[this, &doOnNextCalled](const auto value)
			{
				EXPECT_EQ(this_thread::get_id(), m_workerThreadId);
				doOnNextCalled = true;
				EXPECT_EQ(value, defaultValue);
			})
		.subscribe();
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(doOnNextCalled);
}

TEST_F(MaybeObserveOn, checkDoOnErrorCalledOnWorkerThread)
{
	bool doOnErrorCalled = false;
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.observeOn(*m_worker)
		.doOnError(
			[this, &doOnErrorCalled](const auto &exception)
			{
				EXPECT_EQ(this_thread::get_id(), m_workerThreadId);
				doOnErrorCalled = true;
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
	EXPECT_TRUE(doOnErrorCalled);
}

TEST_F(MaybeObserveOn, checkDoOnCompleteCalledOnWorkerThread)
{
	bool doOnCompleteCalled = false;
	Maybe<int>::empty() //
		.observeOn(*m_worker)
		.doOnComplete(
			[this, &doOnCompleteCalled]()
			{
				EXPECT_EQ(this_thread::get_id(), m_workerThreadId);
				doOnCompleteCalled = true;
			})
		.subscribe();
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(doOnCompleteCalled);
}

class MaybeSubscribeOn : public WorkerThreadBasedTest
{
};

TEST_F(MaybeSubscribeOn, checkSubscribeOnWorkerThread)
{
	bool deferCalled = false;
	Maybe<int>::defer(
		[this, &deferCalled]()
		{
			EXPECT_EQ(this_thread::get_id(), m_workerThreadId);
			deferCalled = true;
			return Maybe<int>::empty();
		}) //
		.subscribeOn(*m_worker)
		.subscribe();
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(deferCalled);
}

TEST_F(MaybeSubscribeOn, checkDoOnNextCalledOnWorkerThread)
{
	bool doOnNextCalled = false;
	Maybe<int>::just(defaultValue) //
		.subscribeOn(*m_worker)
		.doOnNext(
			[this, &doOnNextCalled](const auto value)
			{
				EXPECT_EQ(this_thread::get_id(), m_workerThreadId);
				doOnNextCalled = true;
				EXPECT_EQ(value, defaultValue);
			})
		.subscribe();
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(doOnNextCalled);
}

TEST_F(MaybeSubscribeOn, checkDoOnErrorCalledOnWorkerThread)
{
	bool doOnErrorCalled = false;
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.subscribeOn(*m_worker)
		.doOnError(
			[this, &doOnErrorCalled](const auto &exception)
			{
				EXPECT_EQ(this_thread::get_id(), m_workerThreadId);
				doOnErrorCalled = true;
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
	EXPECT_TRUE(doOnErrorCalled);
}

TEST_F(MaybeSubscribeOn, checkDoOnCompleteCalledOnWorkerThread)
{
	bool doOnCompleteCalled = false;
	Maybe<int>::empty() //
		.subscribeOn(*m_worker)
		.doOnComplete(
			[this, &doOnCompleteCalled]()
			{
				EXPECT_EQ(this_thread::get_id(), m_workerThreadId);
				doOnCompleteCalled = true;
			})
		.subscribe();
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(doOnCompleteCalled);
}

class MaybeDelay : public EventLoopBasedTest
{
};

TEST_F(MaybeDelay, checkOnNextDelay)
{
	recpp::async::Scheduler::TimePoint start;
	recpp::async::Scheduler::TimePoint end;
	Maybe<int>::defer(
		[&start]()
		{
			start = recpp::async::Scheduler::Clock::now();
			return Maybe<int>::just(defaultValue);
		}) //
		.delay(*m_eventLoop, delayDuration, false)
		.subscribe(
			[this, &end](const auto)
			{
				end = recpp::async::Scheduler::Clock::now();
				m_eventLoop->stop();
			});

	m_eventLoop->run();
	auto timeDiff = end - start;
	auto gap = timeDiff - delayDuration;
	EXPECT_LT(chrono::abs(gap), delayTolerance);
}

TEST_F(MaybeDelay, checkOnErrorDelay)
{
	recpp::async::Scheduler::TimePoint start;
	recpp::async::Scheduler::TimePoint end;
	Maybe<int>::defer(
		[&start]()
		{
			start = recpp::async::Scheduler::Clock::now();
			return Maybe<int>::error(runtime_error(runtimeErrorMessage.data()));
		}) //
		.delay(*m_eventLoop, delayDuration, true)
		.subscribe([](const auto) {},
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

TEST_F(MaybeDelay, checkNoErrorDelayMode)
{
	recpp::async::Scheduler::TimePoint start;
	recpp::async::Scheduler::TimePoint end;
	Maybe<int>::defer(
		[&start]()
		{
			start = recpp::async::Scheduler::Clock::now();
			return Maybe<int>::error(runtime_error(runtimeErrorMessage.data()));
		}) //
		.delay(*m_eventLoop, delayDuration, false)
		.subscribe([](const auto) {},
				   [this, &end](const auto &)
				   {
					   end = recpp::async::Scheduler::Clock::now();
					   m_eventLoop->stop();
				   });

	m_eventLoop->run();
	auto timeDiff = end - start;
	EXPECT_LT(chrono::abs(timeDiff), delayTolerance);
}

TEST_F(MaybeDelay, checkOnCompleteDelay)
{
	recpp::async::Scheduler::TimePoint start;
	recpp::async::Scheduler::TimePoint end;
	Maybe<int>::defer(
		[&start]()
		{
			start = recpp::async::Scheduler::Clock::now();
			return Maybe<int>::just(defaultValue);
		}) //
		.delay(*m_eventLoop, delayDuration, true)
		.subscribe([](const auto) {}, [](const auto &) {},
				   [this, &end]()
				   {
					   end = recpp::async::Scheduler::Clock::now();
					   m_eventLoop->stop();
				   });

	m_eventLoop->run();
	auto timeDiff = end - start;
	auto gap = timeDiff - delayDuration;
	EXPECT_LT(chrono::abs(gap), delayTolerance);
}

class MaybeSwitchIfEmpty : public testing::Test
{
};

TEST_F(MaybeSwitchIfEmpty, checkValueIsForwarded)
{
	bool gotValue = false;
	Maybe<int>::just(defaultValue) //
		.switchIfEmpty(otherValue)
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, defaultValue);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(MaybeSwitchIfEmpty, checkDefaultValueIsEmitedIfEmpty)
{
	bool gotValue = false;
	Maybe<int>::empty() //
		.switchIfEmpty(otherValue)
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, otherValue);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(MaybeSwitchIfEmpty, checkErrorsAreForwarded)
{
	bool gotError = false;
	Maybe<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.switchIfEmpty(otherValue)
		.subscribe([](const auto) {},
				   [&gotError](const auto &exception)
				   {
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
