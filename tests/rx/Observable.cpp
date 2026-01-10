// fixtures
#include <fixtures/EventLoopBasedTest.h>
#include <fixtures/WorkerThreadBasedTest.h>

// gtest
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// recpp
#include <recpp/async/EventLoop.h>
#include <recpp/async/WorkerThread.h>
#include <recpp/rx/Observable.h>

// stl
#include <array>
#include <iostream>
#include <ranges>

using namespace recpp::async;
using namespace recpp::rx;
using namespace std;

namespace
{
	constexpr auto			sleepDuration = chrono::milliseconds(10);
	constexpr int			defaultValue = 42;
	constexpr int			otherValue = 55;
	const vector<int>		defaultValues = {1, 2, 3};
	constexpr int			defaultValuesMin = 1;
	constexpr int			defaultValuesMax = 3;
	constexpr int			defaultValuesCount = 3;
	const vector<int>		mergedDefaultValues = {1, 1, 1, 2, 2, 2, 3, 3, 3};
	constexpr array<int, 3> evenValues({2, 4, 6});
	constexpr int			evenValuesSum = 12;
	constexpr array<int, 3> oddValues({1, 3, 5});
	constexpr auto			delayTolerance = chrono::milliseconds(1);
	constexpr auto			delayDuration = chrono::milliseconds(10);
	constexpr auto			sleepDurationForDelay = chrono::milliseconds(150);
	constexpr string_view	runtimeErrorMessage = "unexpected error!";

	bool isOdd(int value)
	{
		return value % 2 == 1;
	}

	bool isEven(int value)
	{
		return value % 2 == 0;
	}

	float divideByTen(int value)
	{
		return value / 10.f;
	}
} // namespace

class ObservableCreate : public testing::Test
{
protected:
	static Observable<int> listOfValues()
	{
		return Observable<int>::create(
			[](auto &subscriber)
			{
				for (const auto value : defaultValues)
					subscriber.onNext(value);
				subscriber.onComplete();
			});
	}
	static Observable<int> completesTwice()
	{
		return Observable<int>::create(
			[](auto &subscriber)
			{
				for (const auto value : defaultValues)
					subscriber.onNext(value);
				subscriber.onComplete();
				subscriber.onComplete();
			});
	}
	static Observable<int> valueAfterCompletion()
	{
		return Observable<int>::create(
			[](auto &subscriber)
			{
				subscriber.onComplete();
				subscriber.onNext(42);
			});
	}
	static Observable<int> errored()
	{
		return Observable<int>::create(
			[](auto &subscriber)
			{
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
			});
	}
	static Observable<int> valueAfterError()
	{
		return Observable<int>::create(
			[](auto &subscriber)
			{
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
				subscriber.onNext(42);
			});
	}
	static Observable<int> doubleError()
	{
		return Observable<int>::create(
			[](auto &subscriber)
			{
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
			});
	}
	static Observable<int> completeAfterError()
	{
		return Observable<int>::create(
			[](auto &subscriber)
			{
				subscriber.onError(runtime_error(runtimeErrorMessage.data()));
				subscriber.onComplete();
			});
	}
};

TEST_F(ObservableCreate, checkEmitedValues)
{
	vector<int> result;
	bool		completed = false;
	listOfValues() //
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

TEST_F(ObservableCreate, checkOnCompleteIsEmited)
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

TEST_F(ObservableCreate, checkOnCompleteIsNotEmitedTwice)
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

TEST_F(ObservableCreate, checkValuesAfterCompleteArentEmited)
{
	valueAfterCompletion() //
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

TEST_F(ObservableCreate, checkEmitedError)
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

TEST_F(ObservableCreate, checkValuesAfterErrorArentEmited)
{
	valueAfterError() //
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

TEST_F(ObservableCreate, checkCompleteAfterErrorIsntEmited)
{
	completeAfterError() //
		.subscribe([](const auto) {}, [](const auto &) {},
				   []()
				   {
					   ADD_FAILURE();
				   });
}

class ObservableDefer : public testing::Test
{
protected:
	static Observable<int> deferredRange()
	{
		return Observable<int>::defer(
			[]()
			{
				return Observable<int>::range(defaultValues);
			});
	}
	static Observable<int> deferredEmpty()
	{
		return Observable<int>::defer(
			[]()
			{
				return Observable<int>::empty();
			});
	}
	static Observable<int> deferredError()
	{
		return Observable<int>::defer(
			[]()
			{
				return Observable<int>::error(runtime_error(runtimeErrorMessage.data()));
			});
	}
};

TEST_F(ObservableDefer, checkEmitedValues)
{
	vector<int> result;
	bool		completed = false;
	deferredRange() //
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

TEST_F(ObservableDefer, checkOnCompleteIsEmited)
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

TEST_F(ObservableDefer, checkEmitedError)
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

class ObservableEmpty : public testing::Test
{
};

TEST_F(ObservableEmpty, checkOnCompleteIsEmited)
{
	bool completed = false;
	Observable<int>::empty() //
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&completed]()
				   {
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(ObservableEmpty, checkOnCompleteIsNotCalledTwice)
{
	bool completed = false;
	Observable<int>::empty() //
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&completed]()
				   {
					   EXPECT_FALSE(completed);
					   completed = true;
				   });
}

TEST_F(ObservableEmpty, checkNoValueIsEmited)
{
	Observable<int>::empty() //
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

TEST_F(ObservableEmpty, checkNoErrorIsEmited)
{
	Observable<int>::empty() //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

class ObservableError : public testing::Test
{
protected:
	static Observable<int> errored()
	{
		return Observable<int>::error(runtime_error(runtimeErrorMessage.data()));
	}
};

TEST_F(ObservableError, checkOnErrorIsEmited)
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

TEST_F(ObservableError, checkOnlyOneErrorIsEmited)
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

TEST_F(ObservableError, checkNoValueIsEmited)
{
	errored() //
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			});
}

TEST_F(ObservableError, checkOnCompleteIsntEmited)
{
	errored() //
		.subscribe([](const auto) {}, [](const auto &) {},
				   []()
				   {
					   ADD_FAILURE();
				   });
}

class ObservableJust : public testing::Test
{
};

TEST_F(ObservableJust, checkValueIsEmitted)
{
	bool gotValue = false;
	Observable<int>::just(defaultValue)
		.subscribe(
			[&gotValue](const auto value)
			{
				EXPECT_EQ(value, defaultValue);
				gotValue = true;
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableJust, checkOnlyOneValueIsEmitted)
{
	bool gotValue = false;
	Observable<int>::just(defaultValue)
		.subscribe(
			[&gotValue](const auto value)
			{
				EXPECT_FALSE(gotValue);
				gotValue = true;
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableJust, checkOnCompleteIsEmited)
{
	bool completed = false;
	Observable<int>::just(defaultValue)
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&completed]()
				   {
					   EXPECT_FALSE(completed);
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(ObservableJust, checkNoError)
{
	Observable<int>::just(defaultValue)
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

class ObservableNever : public testing::Test
{
};

TEST_F(ObservableNever, checkNothingIsEmited)
{
	Observable<int>::never().subscribe(
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

class ObservableRange : public testing::Test
{
};

TEST_F(ObservableRange, checkEmitedValues)
{
	vector<int> result;
	bool		completed = false;
	Observable<int>::range(defaultValues) //
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

TEST_F(ObservableRange, checkOnCompleteIsEmited)
{
	bool completed = false;
	Observable<int>::range(defaultValues) //
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&completed]()
				   {
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(ObservableRange, checkNoErrorIsEmited)
{
	Observable<int>::range(defaultValues) //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

class ObservableMerge : public testing::Test
{
protected:
	static Observable<Observable<int>> observableOfObservable()
	{
		return Observable<Observable<int>>::create(
			[](auto &subscriber)
			{
				subscriber.onNext(Observable<int>::range(defaultValues));
				subscriber.onNext(Observable<int>::range(defaultValues));
				subscriber.onNext(Observable<int>::range(defaultValues));
				subscriber.onComplete();
			});
	}
	static Observable<Observable<int>> observableOfObservableWithError()
	{
		return Observable<Observable<int>>::create(
			[](auto &subscriber)
			{
				subscriber.onNext(Observable<int>::range(defaultValues));
				subscriber.onNext(Observable<int>::error(runtime_error(runtimeErrorMessage.data())));
				subscriber.onNext(Observable<int>::range(defaultValues));
				subscriber.onComplete();
			});
	}
};

TEST_F(ObservableMerge, checkEmitedValues)
{
	vector<int> result;
	bool		completed = false;
	auto		source = observableOfObservable();
	Observable<int>::merge(source) //
		.subscribe(
			[&result](const auto value)
			{
				result.push_back(value);
			},
			[](const auto &) {},
			[&completed, &result]()
			{
				completed = true;
				EXPECT_EQ(result, mergedDefaultValues);
			});
	EXPECT_TRUE(completed);
}

TEST_F(ObservableMerge, checkOnCompleteIsEmited)
{
	bool completed = false;
	auto source = observableOfObservable();
	Observable<int>::merge(source) //
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&completed]()
				   {
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(ObservableMerge, checkNoErrorIsEmited)
{
	auto source = observableOfObservable();
	Observable<int>::merge(source) //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(ObservableMerge, checkErrorCanBeEmited)
{
	bool gotError = false;
	auto source = observableOfObservableWithError();
	Observable<int>::merge(source) //
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

class ObservableInterval : public EventLoopBasedTest
{
};

TEST_F(ObservableInterval, checkEmitedValues)
{
	const vector<int> expected = {0, 1, 2, 3, 4};
	vector<int>		  values;
	Observable<int>::interval(chrono::milliseconds(1), *m_eventLoop) //
		.subscribe(
			[this, &values](const auto value)
			{
				values.push_back(value);
				if (value == 4)
					m_eventLoop->stop();
			});
	m_eventLoop->run();
	EXPECT_EQ(values, expected);
}

TEST_F(ObservableInterval, checkNoErrorIsEmited)
{
	Observable<int>::interval(chrono::milliseconds(1), *m_eventLoop) //
		.subscribe(
			[this](const auto value)
			{
				if (value == 4)
					m_eventLoop->stop();
			},
			[](const auto &)
			{
				ADD_FAILURE();
			});
	m_eventLoop->run();
}

class ObservableFilter : public testing::Test
{
};

TEST_F(ObservableFilter, checkEmitedValues)
{
	bool			  completed = false;
	const vector<int> expected = {1, 3};
	vector<int>		  values;
	Observable<int>::range(defaultValues) //
		.filter(&isOdd)
		.subscribe(
			[&values](const auto value)
			{
				values.push_back(value);
			},
			[](const auto &) {},
			[&values, &expected, &completed]()
			{
				EXPECT_EQ(values, expected);
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(ObservableFilter, checkNoErrorIsEmited)
{
	Observable<int>::range(defaultValues) //
		.filter(&isOdd)
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(ObservableFilter, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.filter(&isOdd)
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

TEST_F(ObservableFilter, checkDoNotCompleteOnError)
{
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.filter(&isOdd)
		.subscribe([](const auto) {}, [](const auto &) {},
				   []()
				   {
					   ADD_FAILURE();
				   });
}

class ObservableTake : public EventLoopBasedTest
{
protected:
	Observable<int> takeObservable()
	{
		return Observable<int>::interval(chrono::milliseconds(1), *m_eventLoop).take(3);
	}
};

TEST_F(ObservableTake, checkEmitedValues)
{
	bool			  completed = false;
	const vector<int> expected = {0, 1, 2};
	vector<int>		  values;
	takeObservable() //
		.subscribe(
			[&values](const auto value)
			{
				values.push_back(value);
			},
			[](const auto &) {},
			[this, &values, &expected, &completed]()
			{
				EXPECT_EQ(values, expected);
				completed = true;
				m_eventLoop->stop();
			});
	m_eventLoop->run();
	EXPECT_TRUE(completed);
}

TEST_F(ObservableTake, checkNoErrorIsEmited)
{
	takeObservable() //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   },
				   [this]()
				   {
					   m_eventLoop->stop();
				   });
	m_eventLoop->run();
}

TEST_F(ObservableTake, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.take(3)
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

TEST_F(ObservableTake, checkDoNotCompleteOnError)
{
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.take(3)
		.subscribe([](const auto) {}, [](const auto &) {},
				   []()
				   {
					   ADD_FAILURE();
				   });
}

class ObservableTakeWhile : public EventLoopBasedTest
{
protected:
	Observable<int> takeWhileObservable()
	{
		return Observable<int>::interval(chrono::milliseconds(1), *m_eventLoop)
			.takeWhile(
				[](const auto value)
				{
					return value < 3;
				});
	}
};

TEST_F(ObservableTakeWhile, checkEmitedValues)
{
	bool			  completed = false;
	const vector<int> expected = {0, 1, 2};
	vector<int>		  values;
	takeWhileObservable() //
		.subscribe(
			[&values](const auto value)
			{
				values.push_back(value);
			},
			[](const auto &) {},
			[this, &values, &expected, &completed]()
			{
				EXPECT_EQ(values, expected);
				completed = true;
				m_eventLoop->stop();
			});
	m_eventLoop->run();
	EXPECT_TRUE(completed);
}

TEST_F(ObservableTakeWhile, checkNoErrorIsEmited)
{
	takeWhileObservable() //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   },
				   [this]()
				   {
					   m_eventLoop->stop();
				   });
	m_eventLoop->run();
}

TEST_F(ObservableTakeWhile, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.takeWhile(
			[](const auto value)
			{
				return value < 3;
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

TEST_F(ObservableTakeWhile, checkDoNotCompleteOnError)
{
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.takeWhile(
			[](const auto value)
			{
				return value < 3;
			})
		.subscribe([](const auto) {}, [](const auto &) {},
				   []()
				   {
					   ADD_FAILURE();
				   });
}

class ObservableTakeUntil : public EventLoopBasedTest
{
protected:
	Observable<int> takeUntilObservable()
	{
		return Observable<int>::interval(chrono::milliseconds(1), *m_eventLoop)
			.takeUntil(
				[](const auto value)
				{
					return value > 2;
				});
	}
};

TEST_F(ObservableTakeUntil, checkEmitedValues)
{
	bool			  completed = false;
	const vector<int> expected = {0, 1, 2};
	vector<int>		  values;
	takeUntilObservable() //
		.subscribe(
			[&values](const auto value)
			{
				values.push_back(value);
			},
			[](const auto &) {},
			[this, &values, &expected, &completed]()
			{
				EXPECT_EQ(values, expected);
				completed = true;
				m_eventLoop->stop();
			});
	m_eventLoop->run();
	EXPECT_TRUE(completed);
}

TEST_F(ObservableTakeUntil, checkNoErrorIsEmited)
{
	takeUntilObservable() //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   },
				   [this]()
				   {
					   m_eventLoop->stop();
				   });
	m_eventLoop->run();
}

TEST_F(ObservableTakeUntil, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.takeWhile(
			[](const auto value)
			{
				return value < 3;
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

TEST_F(ObservableTakeUntil, checkDoNotCompleteOnError)
{
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.takeWhile(
			[](const auto value)
			{
				return value < 3;
			})
		.subscribe([](const auto) {}, [](const auto &) {},
				   []()
				   {
					   ADD_FAILURE();
				   });
}

class ObservableIgnoreElements : public testing::Test
{
};

TEST_F(ObservableIgnoreElements, checkCompletes)
{
	bool completed = false;
	Observable<int>::range(defaultValues) //
		.ignoreElements()
		.subscribe(
			[&completed]()
			{
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(ObservableIgnoreElements, checkNoErrorIsEmited)
{
	Observable<int>::range(defaultValues) //
		.ignoreElements()
		.subscribe([]() {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(ObservableIgnoreElements, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.ignoreElements()
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

TEST_F(ObservableIgnoreElements, checkDoNotCompleteOnError)
{
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.ignoreElements()
		.subscribe(
			[]()
			{
				ADD_FAILURE();
			});
}

class ObservableMap : public testing::Test
{
};

TEST_F(ObservableMap, checkEmitedValues)
{
	bool				completed = false;
	const vector<float> expected = {0.1f, 0.2f, 0.3f};
	vector<float>		values;
	Observable<int>::range(defaultValues) //
		.template map<float>(&divideByTen)
		.subscribe(
			[&values](const auto value)
			{
				values.push_back(value);
			},
			[](const auto &) {},
			[&values, &expected, &completed]()
			{
				EXPECT_EQ(values, expected);
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(ObservableMap, checkNoErrorIsEmited)
{
	Observable<int>::range(defaultValues) //
		.template map<float>(&divideByTen)
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(ObservableMap, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
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

TEST_F(ObservableMap, checkDoNotCompleteOnError)
{
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.template map<float>(&divideByTen)
		.subscribe([](const auto) {}, [](const auto &) {},
				   []()
				   {
					   ADD_FAILURE();
				   });
}

class ObservableFlatMap : public testing::Test
{
protected:
	static Observable<float> transformedValues()
	{
		return Observable<int>::range(defaultValues) //
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
				});
	}
};

TEST_F(ObservableFlatMap, checkEmitedValues)
{
	bool				completed = false;
	const vector<float> expected({2.f, 3.f, 4.f, 3.f, 4.f, 5.f, 4.f, 5.f, 6.f});
	vector<float>		values;
	transformedValues() //
		.subscribe(
			[&values](const auto value)
			{
				values.push_back(value);
			},
			[](const auto &) {},
			[&values, &expected, &completed]()
			{
				EXPECT_EQ(values, expected);
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(ObservableFlatMap, checkNoErrorIsEmited)
{
	transformedValues() //
		.subscribe([](const auto) {},
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(ObservableFlatMap, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.flatMap<float>(
			[](const auto)
			{
				return Observable<float>::just(defaultValue);
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

TEST_F(ObservableFlatMap, checkDoNotCompleteOnError)
{
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.flatMap<float>(
			[](const auto)
			{
				return Observable<float>::just(defaultValue);
			})
		.subscribe([](const auto) {}, [](const auto &) {},
				   []()
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(ObservableFlatMap, checkCanEmitErrors)
{
	bool gotError = false;
	Observable<int>::just(defaultValue) //
		.flatMap<float>(
			[](const auto)
			{
				return Observable<float>::error(runtime_error(runtimeErrorMessage.data()));
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

class ObservableDoOnComplete : public testing::Test
{
};

TEST_F(ObservableDoOnComplete, checkDoOnCompleteIsCalledOnce)
{
	bool completed = false;
	Observable<int>::range(defaultValues) //
		.doOnComplete(
			[&completed]()
			{
				EXPECT_FALSE(completed);
				completed = true;
			})
		.subscribe();
	EXPECT_TRUE(completed);
}

TEST_F(ObservableDoOnComplete, checkDoOnCompleteIsNotCalledOnError)
{
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.doOnComplete(
			[]()
			{
				ADD_FAILURE();
			})
		.subscribe();
}

class ObservableDoOnError : public testing::Test
{
};

TEST_F(ObservableDoOnError, checkDoOnErrorIsNotCalledWhenNoErrorIsEmited)
{
	Observable<int>::range(defaultValues) //
		.doOnError(
			[](const auto &)
			{
				ADD_FAILURE();
			})
		.subscribe();
}

TEST_F(ObservableDoOnError, checkDoOnErrorIsCalledOnError)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
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

class ObservableDoOnNext : public testing::Test
{
};

TEST_F(ObservableDoOnNext, checkDoOnNextIsCalledForEachValue)
{
	bool		completed = false;
	vector<int> values;
	Observable<int>::range(defaultValues) //
		.doOnNext(
			[&values](const auto value)
			{
				values.push_back(value);
			})
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&values, &completed]()
				   {
					   EXPECT_EQ(values, defaultValues);
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(ObservableDoOnNext, checkDoOnNextIsNotCalledInCaseOfAnError)
{
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.doOnNext(
			[](const auto value)
			{
				ADD_FAILURE();
			})
		.subscribe();
}

class ObservableDoOnTerminate : public testing::Test
{
};

TEST_F(ObservableDoOnTerminate, checkDoOnTerminateIsCalledAfterComplete)
{
	bool terminated = false;
	Observable<int>::range(defaultValues) //
		.doOnTerminate(
			[&terminated]()
			{
				terminated = true;
			})
		.subscribe();
	EXPECT_TRUE(terminated);
}

TEST_F(ObservableDoOnTerminate, checkDoOnTerminateIsCalledOnError)
{
	bool terminated = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.doOnTerminate(
			[&terminated]()
			{
				terminated = true;
			})
		.subscribe();
	EXPECT_TRUE(terminated);
}

class ObservableTap : public testing::Test
{
};

TEST_F(ObservableTap, checkTapDoOnCompleteIsCalledOnce)
{
	bool completed = false;
	Observable<int>::range(defaultValues) //
		.tap([](const auto) {}, [](const auto &) {},
			 [&completed]()
			 {
				 EXPECT_FALSE(completed);
				 completed = true;
			 })
		.subscribe();
	EXPECT_TRUE(completed);
}

TEST_F(ObservableTap, checkTapDoOnCompleteIsNotCalledOnError)
{
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.tap([](const auto) {}, [](const auto &) {},
			 []()
			 {
				 ADD_FAILURE();
			 })
		.subscribe();
}

TEST_F(ObservableTap, checkTapDoOnErrorIsNotCalledWhenNoErrorIsEmited)
{
	Observable<int>::range(defaultValues) //
		.tap([](const auto) {},
			 [](const auto &)
			 {
				 ADD_FAILURE();
			 },
			 []() {})
		.subscribe();
}

TEST_F(ObservableTap, checkTapDoOnErrorIsCalledOnError)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
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

TEST_F(ObservableTap, checkTapDoOnNextIsCalledForEachValue)
{
	bool		completed = false;
	vector<int> values;
	Observable<int>::range(defaultValues) //
		.tap(
			[&values](const auto value)
			{
				values.push_back(value);
			},
			[](const auto &) {}, []() {})
		.subscribe([](const auto) {}, [](const auto &) {},
				   [&values, &completed]()
				   {
					   EXPECT_EQ(values, defaultValues);
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(ObservableTap, checkTapDoOnNextIsNotCalledInCaseOfAnError)
{
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.tap(
			[](const auto value)
			{
				ADD_FAILURE();
			},
			[](const auto &) {}, []() {})
		.subscribe();
}

class ObservableSwitchOnError : public testing::Test
{
};

TEST_F(ObservableSwitchOnError, checkForwardsValues)
{
	vector<int> values;
	Observable<int>::range(defaultValues) //
		.switchOnError(Observable<int>::just(defaultValue))
		.subscribe(
			[&values](const auto value)
			{
				values.push_back(value);
			});
	EXPECT_EQ(values, defaultValues);
}

TEST_F(ObservableSwitchOnError, checkCompletes)
{
	auto completed = false;
	Observable<int>::range(defaultValues) //
		.switchOnError(Observable<int>::just(defaultValue))
		.subscribe(nullptr, nullptr,
				   [&completed]()
				   {
					   EXPECT_FALSE(completed);
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(ObservableSwitchOnError, checkNoError)
{
	Observable<int>::range(defaultValues) //
		.switchOnError(Observable<int>::just(defaultValue))
		.subscribe(nullptr,
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(ObservableSwitchOnError, checkSwitchWithValue)
{
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.switchOnError(Observable<int>::just(defaultValue))
		.subscribe(
			[](const auto value)
			{
				EXPECT_EQ(value, defaultValue);
			});
}

TEST_F(ObservableSwitchOnError, checkSwitchCompletes)
{
	auto completed = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.switchOnError(Observable<int>::just(defaultValue))
		.subscribe(nullptr, nullptr,
				   [&completed]()
				   {
					   EXPECT_FALSE(completed);
					   completed = true;
				   });
	EXPECT_TRUE(completed);
}

TEST_F(ObservableSwitchOnError, checkSwitchNoError)
{
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.switchOnError(Observable<int>::just(defaultValue))
		.subscribe(nullptr,
				   [](const auto &)
				   {
					   ADD_FAILURE();
				   });
}

TEST_F(ObservableSwitchOnError, checkForwardsError)
{
	auto gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.switchOnError(Observable<int>::error(runtime_error(runtimeErrorMessage.data())))
		.subscribe(nullptr,
				   [&gotError](const auto &error)
				   {
					   EXPECT_FALSE(gotError);
					   gotError = true;
					   try
					   {
						   rethrow_exception(error);
					   }
					   catch (runtime_error &runtimeError)
					   {
						   EXPECT_THAT(runtimeError.what(), testing::StrEq(runtimeErrorMessage));
					   }
				   });
	EXPECT_TRUE(gotError);
}

class ObservableObserveOn : public WorkerThreadBasedTest
{
};

TEST_F(ObservableObserveOn, checkSubscribeOnMainThread)
{
	bool deferCalled = false;
	Observable<int>::defer(
		[this, &deferCalled]()
		{
			EXPECT_EQ(this_thread::get_id(), m_mainThreadId);
			deferCalled = true;
			return Observable<int>::empty();
		}) //
		.observeOn(*m_worker)
		.subscribe();
	EXPECT_TRUE(deferCalled);
}

TEST_F(ObservableObserveOn, checkDoOnNextCalledOnWorkerThread)
{
	bool doOnNextCalled = false;
	Observable<int>::just(defaultValue) //
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

TEST_F(ObservableObserveOn, checkDoOnErrorCalledOnWorkerThread)
{
	bool doOnErrorCalled = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
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

TEST_F(ObservableObserveOn, checkDoOnCompleteCalledOnWorkerThread)
{
	bool doOnCompleteCalled = false;
	Observable<int>::empty() //
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

class ObservableSubscribeOn : public WorkerThreadBasedTest
{
};

TEST_F(ObservableSubscribeOn, checkSubscribeOnWorkerThread)
{
	bool deferCalled = false;
	Observable<int>::defer(
		[this, &deferCalled]()
		{
			EXPECT_EQ(this_thread::get_id(), m_workerThreadId);
			deferCalled = true;
			return Observable<int>::empty();
		}) //
		.subscribeOn(*m_worker)
		.subscribe();
	this_thread::sleep_for(sleepDuration);
	EXPECT_TRUE(deferCalled);
}

TEST_F(ObservableSubscribeOn, checkDoOnNextCalledOnWorkerThread)
{
	bool doOnNextCalled = false;
	Observable<int>::just(defaultValue) //
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

TEST_F(ObservableSubscribeOn, checkDoOnErrorCalledOnWorkerThread)
{
	bool doOnErrorCalled = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
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

TEST_F(ObservableSubscribeOn, checkDoOnCompleteCalledOnWorkerThread)
{
	bool doOnCompleteCalled = false;
	Observable<int>::empty() //
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

class ObservableDelay : public EventLoopBasedTest
{
};

TEST_F(ObservableDelay, checkOnNextDelay)
{
	recpp::async::Scheduler::TimePoint start;
	recpp::async::Scheduler::TimePoint end;
	Observable<int>::defer(
		[&start]()
		{
			start = recpp::async::Scheduler::Clock::now();
			return Observable<int>::just(defaultValue);
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

TEST_F(ObservableDelay, checkOnErrorDelay)
{
	recpp::async::Scheduler::TimePoint start;
	recpp::async::Scheduler::TimePoint end;
	Observable<int>::defer(
		[&start]()
		{
			start = recpp::async::Scheduler::Clock::now();
			return Observable<int>::error(runtime_error(runtimeErrorMessage.data()));
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

TEST_F(ObservableDelay, checkNoErrorDelayMode)
{
	recpp::async::Scheduler::TimePoint start;
	recpp::async::Scheduler::TimePoint end;
	Observable<int>::defer(
		[&start]()
		{
			start = recpp::async::Scheduler::Clock::now();
			return Observable<int>::error(runtime_error(runtimeErrorMessage.data()));
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

TEST_F(ObservableDelay, checkOnCompleteDelay)
{
	recpp::async::Scheduler::TimePoint start;
	recpp::async::Scheduler::TimePoint end;
	Observable<int>::defer(
		[&start]()
		{
			start = recpp::async::Scheduler::Clock::now();
			return Observable<int>::just(defaultValue);
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

class ObservableDefaultIfEmpty : public testing::Test
{
};

TEST_F(ObservableDefaultIfEmpty, checkValuesAreForwarded)
{
	bool gotValue = false;
	Observable<int>::just(defaultValue) //
		.defaultIfEmpty(otherValue)
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, defaultValue);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableDefaultIfEmpty, checkDefaultValueIsEmitedIfEmpty)
{
	bool gotValue = false;
	Observable<int>::empty() //
		.defaultIfEmpty(otherValue)
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, otherValue);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableDefaultIfEmpty, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.defaultIfEmpty(otherValue)
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

class ObservableAllOf : public testing::Test
{
};

TEST_F(ObservableAllOf, checkCanReturnTrue)
{
	bool gotValue = false;
	Observable<int>::range(evenValues) //
		.allOf(&isEven)
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_TRUE(value);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableAllOf, checkCanReturnFalse)
{
	bool gotValue = false;
	Observable<int>::range(defaultValues) //
		.allOf(&isEven)
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_FALSE(value);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableAllOf, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.allOf(&isEven)
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

class ObservableAnyOf : public testing::Test
{
};

TEST_F(ObservableAnyOf, checkCanReturnTrue)
{
	bool gotValue = false;
	Observable<int>::range(defaultValues) //
		.anyOf(&isEven)
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_TRUE(value);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableAnyOf, checkCanReturnFalse)
{
	bool gotValue = false;
	Observable<int>::range(oddValues) //
		.anyOf(&isEven)
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_FALSE(value);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableAnyOf, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.anyOf(&isEven)
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

class ObservableNoneOf : public testing::Test
{
};

TEST_F(ObservableNoneOf, checkCanReturnTrue)
{
	bool gotValue = false;
	Observable<int>::range(oddValues) //
		.noneOf(&isEven)
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_TRUE(value);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableNoneOf, checkCanReturnFalse)
{
	bool gotValue = false;
	Observable<int>::range(defaultValues) //
		.noneOf(&isEven)
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_FALSE(value);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableNoneOf, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.noneOf(&isEven)
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

class ObservableReduce : public testing::Test
{
};

TEST_F(ObservableReduce, checkDefaultBehavior)
{
	bool gotValue = false;
	Observable<int>::range(evenValues) //
		.reduce()
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, evenValuesSum);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableReduce, checkWithInitValue)
{
	bool gotValue = false;
	Observable<int>::range(evenValues) //
		.reduce(10)
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, 10 + evenValuesSum);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableReduce, checkWithOperation)
{
	bool gotValue = false;
	Observable<int>::range(evenValues) //
		.reduce(std::minus<int>())
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, -evenValuesSum);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableReduce, checkWithOperationAndInitValue)
{
	bool gotValue = false;
	Observable<int>::range(evenValues) //
		.reduce(std::minus<int>(), 10)
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, 10 - evenValuesSum);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableReduce, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.reduce()
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

class ObservableMax : public testing::Test
{
};

TEST_F(ObservableMax, checkDefaultBehavior)
{
	bool gotValue = false;
	Observable<int>::range(defaultValues) //
		.max()
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, defaultValuesMax);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableMax, checkWithComparator)
{
	bool gotValue = false;
	Observable<int>::range(defaultValues) //
		.max(std::greater<int>())
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, defaultValuesMin);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableMax, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.max()
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

class ObservableMin : public testing::Test
{
};

TEST_F(ObservableMin, checkDefaultBehavior)
{
	bool gotValue = false;
	Observable<int>::range(defaultValues) //
		.min()
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, defaultValuesMin);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableMin, checkWithComparator)
{
	bool gotValue = false;
	Observable<int>::range(defaultValues) //
		.min(std::greater<int>())
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, defaultValuesMax);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableMin, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.min()
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

class ObservableCount : public testing::Test
{
};

TEST_F(ObservableCount, checkFindsCorrectCount)
{
	bool gotValue = false;
	Observable<int>::range(defaultValues) //
		.count()
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, defaultValuesCount);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableCount, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.count()
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

class ObservableElementAt : public testing::Test
{
};

TEST_F(ObservableElementAt, checkFindsCorrectElementAt)
{
	bool gotValue = false;
	Observable<int>::range(defaultValues) //
		.elementAt(1)
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, defaultValues[1]);
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableElementAt, checkIsEmptyForOutOfRangeIndex)
{
	bool completed = false;
	Observable<int>::range(defaultValues) //
		.elementAt(defaultValuesCount)
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			},
			[](const auto &) {},
			[&completed]()
			{
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(ObservableElementAt, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.elementAt(1)
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

class ObservableFirst : public testing::Test
{
};

TEST_F(ObservableFirst, checkFindsCorrectFirstElement)
{
	bool gotValue = false;
	Observable<int>::range(defaultValues) //
		.first()
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, defaultValues.front());
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableFirst, checkIsEmptyForEmptyObservable)
{
	bool completed = false;
	Observable<int>::empty() //
		.first()
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			},
			[](const auto &) {},
			[&completed]()
			{
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(ObservableFirst, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.first()
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

class ObservableLast : public testing::Test
{
};

TEST_F(ObservableLast, checkFindsCorrectFirstElement)
{
	bool gotValue = false;
	Observable<int>::range(defaultValues) //
		.last()
		.subscribe(
			[&gotValue](const auto value)
			{
				gotValue = true;
				EXPECT_EQ(value, defaultValues.back());
			});
	EXPECT_TRUE(gotValue);
}

TEST_F(ObservableLast, checkIsEmptyForEmptyObservable)
{
	bool completed = false;
	Observable<int>::empty() //
		.first()
		.subscribe(
			[](const auto)
			{
				ADD_FAILURE();
			},
			[](const auto &) {},
			[&completed]()
			{
				completed = true;
			});
	EXPECT_TRUE(completed);
}

TEST_F(ObservableLast, checkErrorsAreForwarded)
{
	bool gotError = false;
	Observable<int>::error(runtime_error(runtimeErrorMessage.data())) //
		.first()
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
