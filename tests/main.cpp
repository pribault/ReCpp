#include <recpp/async/Schedulable.h>
#include <recpp/async/ThreadPool.h>
#include <recpp/rx/Maybe.h>
#include <recpp/rx/Observable.h>
#include <recpp/rx/Single.h>

#include <iostream>
#include <ranges>
#include <string>
#include <thread>
#include <vector>

using namespace recpp;
using namespace std;

int main()
{
	ThreadPool pool(4);

	cout << "[" << this_thread::get_id() << "] Hello from main thread!" << endl;

	const vector<int> values({1, 2, 3});
	// Observable<int>::defer([]() { return Observable<int>::just(66); })
	// Observable<int>::just(42)
	// Observable<int>::error(make_exception_ptr(runtime_error("test")))
	// Observable<int>::empty()
	// Observable<int>::never()
	// Observable<int>::create(
	// 	[](auto &subscriber)
	// 	{
	// 		subscriber.onNext(1);
	// 		subscriber.onNext(2);
	// 		subscriber.onNext(3);
	// 		subscriber.onComplete();
	// 	})
	// Observable<int>::range(values.begin(), values.end())
	Observable<int>::range(values)
	// Maybe<int>::just(42)
		.doOnNext([](int value) { cout << "[" << this_thread::get_id() << "] doOnNext: " << value << endl; })
		.map<int>([](int value) { return value + 2; })
		// .filter([](int value) { return value % 2 == 1; })
		// .flatMap<string>([&values](int value)
		// 				 { return Observable<int>::range(values).map<string>([value](int value2) { return to_string(value) + "x" + to_string(value2); }); })
		// .flatMap<string>([](int value)
		// {
		// 	return Observable<string>::never();
		// })
		.subscribeOn(pool)
		.observeOn(pool)
		.subscribe([](auto value) { cout << "[" << this_thread::get_id() << "] value=" << value << endl; },
				   [](const exception_ptr &e)
				   {
					   try
					   {
						   rethrow_exception(e);
					   }
					   catch (const exception &exception)
					   {
						   cerr << "[" << this_thread::get_id() << "] error: " << exception.what() << endl;
					   }
				   },
				   []() { cout << "[" << this_thread::get_id() << "] completed!" << endl; });

	this_thread::sleep_for(chrono::seconds(2));

	return 0;
}
