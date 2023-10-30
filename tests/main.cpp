import recpp;

import <iostream>;
import <ranges>;
import <vector>;
import <string>;

import recpp.processors.Map;

using namespace recpp;
using namespace std;

int main()
{
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
		.map<int>([](int value) { return value + 2; })
		.subscribe([](int value) { cout << "value=" << value << endl; },
				   [](const exception_ptr &e)
				   {
					   try
					   {
						   rethrow_exception(e);
					   }
					   catch (const exception &exception)
					   {
						   cerr << "error: " << exception.what() << endl;
					   }
				   },
				   []() { cout << "completed!" << endl; });
	return 0;
}
