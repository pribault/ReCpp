// fixtures
#include <fixtures/EventLoopBasedTest.h>
#include <fixtures/WorkerThreadBasedTest.h>

// gtest
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// recpp
#include <recpp/async/EventLoop.h>
#include <recpp/async/WorkerThread.h>
#include <recpp/rx/Single.h>

// stl
#include <iostream>

using namespace recpp::async;
using namespace recpp::rx;
using namespace std;

namespace
{
	constexpr int		  defaultValue = 42;
	const vector<int>	  defaultValues = {1, 2, 3};
	constexpr auto		  sleepDuration = chrono::milliseconds(10);
	constexpr auto		  delayTolerance = chrono::milliseconds(1);
	constexpr auto		  delayDuration = chrono::milliseconds(10);
	constexpr string_view runtimeErrorMessage = "unexpected error!";

	float divideByTen(int value)
	{
		return value / 10.f;
	}
} // namespace

class SingleCreate : public testing::Test
{
public:
	static Single<int> singleValue()
	{
		return Single<int>::create(
			[](auto &subscriber)
			{
				subscriber.onNext(defaultValue);
			});
	}
	static Single<int> twoValues()
	{
		return Single<int>::create(
			[](auto &subscriber)
			{
				subscriber.onNext(defaultValue);
				subscriber.onNext(defaultValue);
			});
	}
	static Single<int> error()
	{
		return Single<int>::create(
			[](auto &subscriber)
			{
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
			});
	}
	static Single<int> twoErrors()
	{
		return Single<int>::create(
			[](auto &subscriber)
			{
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
			});
	}
	static Single<int> errorAfterValue()
	{
		return Single<int>::create(
			[](auto &subscriber)
			{
				subscriber.onNext(defaultValue);
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
			});
	}
	static Single<int> valueAfterError()
	{
		return Single<int>::create(
			[](auto &subscriber)
			{
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
				subscriber.onNext(defaultValue);
			});
	}
};

TEST_F(SingleCreate, checkEmitedValue)
{
	bool completed = false;
	singleValue() //
		.subscribe(
			[&completed](const auto value)
			{
				completed = true;
				EXPECT_EQ(value, defaultValue);
			});
	EXPECT_TRUE(completed);
}

TEST_F(SingleCreate, checkOnErrorIsNotEmitedOnNext)
{
	singleValue() //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(SingleCreate, checkCannotEmitMultipleValues)
{
	bool completed = false;
	twoValues() //
		.subscribe(
			[&completed](const auto)
			{
				EXPECT_FALSE(completed);
				completed = true;
			});
}

TEST_F(SingleCreate, checkOnErrorIsEmited)
{
	bool gotError = false;
	error() //
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

TEST_F(SingleCreate, checkOnNextIsNotEmitedOnError)
{
	error() //
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

TEST_F(SingleCreate, checkOnErrorIsEmitedOnlyOnce)
{
	bool gotError = false;
	twoErrors() //
		.subscribe([](const auto) {},
				   [&gotError](const auto &)
				   {
					   EXPECT_FALSE(gotError);
					   gotError = true;
				   });
	EXPECT_TRUE(gotError);
}

TEST_F(SingleCreate, checkOnErrorIsNotEmitedAfterValue)
{
	errorAfterValue() //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(SingleCreate, checkOnNextIsNotEmitedAfterError)
{
	valueAfterError() //
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

class SingleDefer : public testing::Test
{
protected:
	static Single<int> deferredValue()
	{
		return Single<int>::defer(
			[]()
			{
				return Single<int>::just(defaultValue);
			});
	}
	static Single<int> deferredError()
	{
		return Single<int>::defer(
			[]()
			{
				return Single<int>::error(runtime_error(runtimeErrorMessage.data()));
			});
	}
};

TEST_F(SingleDefer, checkEmitedValue)
{
	bool completed = false;
	deferredValue() //
		.subscribe(
			[&completed](const auto value)
			{
				EXPECT_EQ(value, defaultValue);
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(SingleDefer, checkEmitedError)
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

class SingleError : public testing::Test
{
protected:
	static Single<int> errored()
	{
		return Single<int>::error(runtime_error(runtimeErrorMessage.data()));
	}
};

TEST_F(SingleError, checkOnErrorIsEmited)
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

TEST_F(SingleError, checkOnlyOneErrorIsEmited)
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

TEST_F(SingleError, checkNoValueIsEmited)
{
	errored() //
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

class SingleJust : public testing::Test
{
};

TEST_F(SingleJust, checkValueIsEmitted)
{
	bool gotValue = false;
	Single<int>::just(defaultValue)
		.subscribe(
			[&gotValue](const auto value)
			{
				EXPECT_EQ(value, defaultValue);
				gotValue = true;
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(SingleJust, checkOnlyOneValueIsEmitted)
{
	bool gotValue = false;
	Single<int>::just(defaultValue)
		.subscribe(
			[&gotValue](const auto value)
			{
				EXPECT_FALSE(gotValue);
				gotValue = true;
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(SingleJust, checkNoError)
{
	Single<int>::just(defaultValue)
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

class SingleNever : public testing::Test
{
};

TEST_F(SingleNever, checkNothingIsEmited)
{
	Single<int>::never().subscribe(
		[](const auto)
		{
			ADD_FAILURE();
		},
		[](const auto &)
		{
			ADD_FAILURE();
		});
}

class SingleMap : public testing::Test
{
};

TEST_F(SingleMap, checkEmitedValue)
{
	bool completed = false;
	Single<int>::just(defaultValue) //
		.template map<float>(&divideByTen)
		.subscribe(
			[&completed](const auto value)
			{
				EXPECT_EQ(value, divideByTen(defaultValue));
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(SingleMap, checkNoErrorIsEmited)
{
	Single<int>::just(defaultValue) //
		.template map<float>(&divideByTen)
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(SingleMap, checkErrorsAreForwarded)
{
	bool gotError = false;
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
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
				   });
	EXPECT_TRUE(gotError);
}

TEST_F(SingleMap, checkDoNotEmitValueOnError)
{
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.template map<float>(&divideByTen)
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

class SingleFlatMap : public testing::Test
{
protected:
	static Single<float> transformedValue()
	{
		return Single<int>::just(defaultValue) //
			.flatMap<float>(
				[](const auto value)
				{
					return Single<float>::just(divideByTen(value));
				});
	}
};

TEST_F(SingleFlatMap, checkEmitedValue)
{
	bool completed = false;
	transformedValue() //
		.subscribe(
			[&completed](const auto value)
			{
				EXPECT_EQ(value, divideByTen(defaultValue));
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(SingleFlatMap, checkNoErrorIsEmited)
{
	transformedValue() //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(SingleFlatMap, checkErrorsAreForwarded)
{
	bool gotError = false;
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.flatMap<float>(
			[](const auto)
			{
				return Single<float>::just(defaultValue);
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

TEST_F(SingleFlatMap, checkDoNotEmitValueOnError)
{
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.flatMap<float>(
			[](const auto)
			{
				return Single<float>::just(defaultValue);
			})
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

TEST_F(SingleFlatMap, checkCanEmitErrors)
{
	bool gotError = false;
	Single<int>::just(defaultValue) //
		.flatMap<float>(
			[](const auto)
			{
				return Single<float>::error(runtime_error(runtimeErrorMessage.data()));
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

class SingleFlatMapCompletable : public testing::Test
{
};

TEST_F(SingleFlatMapCompletable, checkOnCompleteIsEmited)
{
	bool completed = false;
	Single<int>::just(42)
		.flatMapCompletable(
			[](int)
			{
				return Completable::complete();
			})
		.subscribe(
			[&completed]()
			{
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(SingleFlatMapCompletable, checkNoErrorIsEmited)
{
	Single<int>::just(42)
		.flatMapCompletable(
			[](int)
			{
				return Completable::complete();
			})
		.subscribe([]() {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(SingleFlatMapCompletable, checkErrorsAreForwarded)
{
	bool gotError = false;
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.flatMapCompletable(
			[](int)
			{
				return Completable::complete();
			})
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

TEST_F(SingleFlatMapCompletable, checkDoNotCompleteOnError)
{
	Single<int>::just(42)
		.flatMapCompletable(
			[](int)
			{
				return Completable::error(runtime_error(runtimeErrorMessage.data()));
			})
		.subscribe(
			[]()
			{
				ADD_FAILURE();
			});
}

class SingleFlatMapMaybe : public testing::Test
{
protected:
	static Maybe<float> maybeWithValue()
	{
		return Single<int>::just(defaultValue) //
			.flatMapMaybe<float>(
				[](const auto value)
				{
					return Maybe<float>::just(divideByTen(value));
				});
	}

	static Maybe<float> maybeWithoutValue()
	{
		return Single<int>::just(defaultValue) //
			.flatMapMaybe<float>(
				[](const auto value)
				{
					return Maybe<float>::empty();
				});
	}
};

TEST_F(SingleFlatMapMaybe, checkEmitedValue)
{
	bool completed = false;
	maybeWithValue() //
		.subscribe(
			[&completed](const auto value)
			{
				EXPECT_EQ(value, divideByTen(defaultValue));
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(SingleFlatMapMaybe, checkNoErrorIsEmitedWithValue)
{
	maybeWithValue() //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(SingleFlatMapMaybe, checkNoErrorIsEmitedWithoutValue)
{
	maybeWithoutValue() //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(SingleFlatMapMaybe, checkErrorsAreForwarded)
{
	bool gotError = false;
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.flatMapMaybe<float>(
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

TEST_F(SingleFlatMapMaybe, checkDoNotEmitValueOnError)
{
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.flatMapMaybe<float>(
			[](const auto)
			{
				return Maybe<float>::just(defaultValue);
			})
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

TEST_F(SingleFlatMapMaybe, checkCanEmitErrors)
{
	bool gotError = false;
	Single<int>::just(defaultValue) //
		.flatMapMaybe<float>(
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

class SingleFlatMapObservable : public testing::Test
{
protected:
	static Observable<int> transformedValues()
	{
		return Single<int>::just(defaultValue) //
			.flatMapObservable<int>(
				[](const auto value)
				{
					return Observable<int>::range(defaultValues);
				});
	}
};

TEST_F(SingleFlatMapObservable, checkEmitedValues)
{
	bool		completed = false;
	vector<int> result;
	transformedValues() //
		.subscribe(
			[&completed, &result](const auto value)
			{
				result.push_back(value);
				completed = true;
			});
	EXPECT_EQ(result, defaultValues);
	EXPECT_TRUE(completed);
}

TEST_F(SingleFlatMapObservable, checkNoErrorIsEmited)
{
	transformedValues() //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(SingleFlatMapObservable, checkErrorsAreForwarded)
{
	bool gotError = false;
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.flatMapObservable<int>(
			[](const auto)
			{
				return Observable<int>::range(defaultValues);
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

TEST_F(SingleFlatMapObservable, checkDoNotEmitValueOnError)
{
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.flatMapObservable<int>(
			[](const auto)
			{
				return Observable<int>::range(defaultValues);
			})
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

TEST_F(SingleFlatMapObservable, checkCanEmitErrors)
{
	bool gotError = false;
	Single<int>::just(defaultValue) //
		.flatMapObservable<int>(
			[](const auto)
			{
				return Observable<int>::error(runtime_error(runtimeErrorMessage.data()));
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

class SingleMerge : public testing::Test
{
protected:
	static Observable<Single<int>> observableOfSingle()
	{
		return Observable<Single<int>>::create(
			[](auto &subscriber)
			{
				subscriber.onNext(Single<int>::just(defaultValues[0]));
				subscriber.onNext(Single<int>::just(defaultValues[1]));
				subscriber.onNext(Single<int>::just(defaultValues[2]));
				subscriber.onComplete();
			});
	}
	static Observable<Single<int>> observableOfSingleWithError()
	{
		return Observable<Single<int>>::create(
			[](auto &subscriber)
			{
				subscriber.onNext(Single<int>::just(defaultValues[0]));
				subscriber.onNext(Single<int>::error(runtime_error(runtimeErrorMessage.data())));
				subscriber.onNext(Single<int>::just(defaultValues[2]));
				subscriber.onComplete();
			});
	}
};

TEST_F(SingleMerge, checkEmitedValues)
{
	vector<int> result;
	bool		completed = false;
	auto		source = observableOfSingle();
	Single<int>::merge(source) //
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

TEST_F(SingleMerge, checkOnCompleteIsEmited)
{
	bool completed = false;
	auto source = observableOfSingle();
	Single<int>::merge(source) //
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&completed]()
				   {
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(SingleMerge, checkNoErrorIsEmited)
{
	auto source = observableOfSingle();
	Single<int>::merge(source) //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(SingleMerge, checkErrorCanBeEmited)
{
	bool gotError = false;
	auto source = observableOfSingleWithError();
	Single<int>::merge(source) //
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

class SingleIgnoreElement : public testing::Test
{
};

TEST_F(SingleIgnoreElement, checkCompletes)
{
	bool completed = false;
	Single<int>::just(defaultValue) //
		.ignoreElement()
		.subscribe(
			[&completed]()
			{
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(SingleIgnoreElement, checkNoErrorIsEmited)
{
	Single<int>::just(defaultValue) //
		.ignoreElement()
		.subscribe([]() {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(SingleIgnoreElement, checkErrorsAreForwarded)
{
	bool gotError = false;
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
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

TEST_F(SingleIgnoreElement, checkDoNotCompleteOnError)
{
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.ignoreElement()
		.subscribe(
			[]()
			{
				ADD_FAILURE();
			});
}

class SingleDoOnError : public testing::Test
{
};

TEST_F(SingleDoOnError, checkDoOnErrorIsNotCalledWhenNoErrorIsEmited)
{
	Single<int>::just(defaultValue) //
		.doOnError(
			[](const auto &)
			{
				ADD_FAILURE();
			})
		.subscribe();
}

TEST_F(SingleDoOnError, checkDoOnErrorIsCalledOnError)
{
	bool gotError = false;
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
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

class SingleDoOnSuccess : public testing::Test
{
};

TEST_F(SingleDoOnSuccess, checkDoOnSuccessIsCalled)
{
	bool completed = false;
	Single<int>::just(defaultValue) //
		.doOnSuccess(
			[&completed](const auto value)
			{
				EXPECT_EQ(value, defaultValue);
				completed = true;
			})
		.subscribe();
	EXPECT_TRUE(completed);
}

TEST_F(SingleDoOnSuccess, checkDoOnSuccessIsNotCalledInCaseOfAnError)
{
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.doOnSuccess(
			[](const auto value)
			{
				ADD_FAILURE();
			})
		.subscribe();
}

class SingleDoOnTerminate : public testing::Test
{
};

TEST_F(SingleDoOnTerminate, checkDoOnTerminateIsCalledAfterComplete)
{
	bool terminated = false;
	Single<int>::just(defaultValue) //
		.doOnTerminate(
			[&terminated]()
			{
				terminated = true;
			})
		.subscribe();
	EXPECT_TRUE(terminated);
}

TEST_F(SingleDoOnTerminate, checkDoOnTerminateIsCalledOnError)
{
	bool terminated = false;
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.doOnTerminate(
			[&terminated]()
			{
				terminated = true;
			})
		.subscribe();
	EXPECT_TRUE(terminated);
}

class SingleTap : public testing::Test
{
};

TEST_F(SingleTap, checkTapDoOnErrorIsNotCalledWhenNoErrorIsEmited)
{
	Single<int>::just(defaultValue) //
		.tap([](const auto) {},
			 [](const auto &)
			 {
				 ADD_FAILURE();
			 })
		.subscribe();
}

TEST_F(SingleTap, checkTapDoOnErrorIsCalledOnError)
{
	bool gotError = false;
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
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
			 })
		.subscribe();
	EXPECT_TRUE(gotError);
}

TEST_F(SingleTap, checkTapDoOnNextIsCalled)
{
	bool completed = false;
	Single<int>::just(defaultValue) //
		.tap(
			[&completed](const auto value)
			{
				EXPECT_EQ(value, defaultValue);
				completed = true;
			},
			[](const auto &) {})
		.subscribe();
	EXPECT_TRUE(completed);
}

TEST_F(SingleTap, checkTapDoOnNextIsNotCalledInCaseOfAnError)
{
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.tap(
			[](const auto)
			{
				ADD_FAILURE();
			},
			[](const auto &) {})
		.subscribe();
}

class SingleObserveOn : public WorkerThreadBasedTest
{
};

TEST_F(SingleObserveOn, checkSubscribeOnMainThread)
{
	bool deferCalled = false;
	Single<int>::defer(
		[this, &deferCalled]()
		{
			EXPECT_EQ(this_thread::get_id(), m_mainThreadId);
			deferCalled = true;
			return Single<int>::just(defaultValue);
		}) //
		.observeOn(*m_worker)
		.subscribe();
	EXPECT_TRUE(deferCalled);
}

TEST_F(SingleObserveOn, checkDoOnSuccessCalledOnWorkerThread)
{
	bool doOnNextCalled = false;
	Single<int>::just(defaultValue) //
		.observeOn(*m_worker)
		.doOnSuccess(
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

TEST_F(SingleObserveOn, checkDoOnErrorCalledOnWorkerThread)
{
	bool doOnErrorCalled = false;
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
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

class SingleSubscribeOn : public WorkerThreadBasedTest
{
};

TEST_F(SingleSubscribeOn, checkSubscribeOnWorkerThread)
{
	bool deferCalled = false;
	Single<int>::defer(
		[this, &deferCalled]()
		{
			EXPECT_EQ(this_thread::get_id(), m_workerThreadId);
			deferCalled = true;
			return Single<int>::just(defaultValue);
		}) //
		.subscribeOn(*m_worker)
		.subscribe();
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(deferCalled);
}

TEST_F(SingleSubscribeOn, checkDoOnSuccessCalledOnWorkerThread)
{
	bool doOnNextCalled = false;
	Single<int>::just(defaultValue) //
		.subscribeOn(*m_worker)
		.doOnSuccess(
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

TEST_F(SingleSubscribeOn, checkDoOnErrorCalledOnWorkerThread)
{
	bool doOnErrorCalled = false;
	Single<int>::error(runtime_error(runtimeErrorMessage.data())) //
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

class SingleDelay : public EventLoopBasedTest
{
};

TEST_F(SingleDelay, checkOnNextDelay)
{
	recpp::async::Scheduler::TimePoint start;
	recpp::async::Scheduler::TimePoint end;
	Single<int>::defer(
		[&start]()
		{
			start = recpp::async::Scheduler::Clock::now();
			return Single<int>::just(defaultValue);
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

TEST_F(SingleDelay, checkOnErrorDelay)
{
	recpp::async::Scheduler::TimePoint start;
	recpp::async::Scheduler::TimePoint end;
	Single<int>::defer(
		[&start]()
		{
			start = recpp::async::Scheduler::Clock::now();
			return Single<int>::error(runtime_error(runtimeErrorMessage.data()));
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

TEST_F(SingleDelay, checkNoErrorDelayMode)
{
	recpp::async::Scheduler::TimePoint start;
	recpp::async::Scheduler::TimePoint end;
	Single<int>::defer(
		[&start]()
		{
			start = recpp::async::Scheduler::Clock::now();
			return Single<int>::error(runtime_error(runtimeErrorMessage.data()));
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
