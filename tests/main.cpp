import recpp;

import <iostream>;
import <ranges>;
import <vector>;

using namespace recpp;

int main()
{
	const std::vector<int> values({1, 2, 3});
	// Observable<int>::defer([]() { return Observable<int>::just(66); })
	// Observable<int>::just(42)
	// Observable<int>::error(std::make_exception_ptr(std::runtime_error("test")))
	// Observable<int>::empty()
	// Observable<int>::never()
	// Observable<int>::create(
	// 	[](const auto &subscriber)
	// 	{
	// 		subscriber.onNext(1);
	// 		subscriber.onNext(2);
	// 		subscriber.onNext(3);
	// 		subscriber.onComplete();
	// 	})
	// Observable<int>::range(values.begin(), values.end())
	Observable<int>::range(values).subscribe([](int value) { std::cout << "value=" << value << std::endl; },
											 [](const std::exception_ptr &e)
											 {
												 try
												 {
													 std::rethrow_exception(e);
												 }
												 catch (const std::exception &exception)
												 {
													 std::cerr << "error: " << exception.what() << std::endl;
												 }
											 },
											 []() { std::cout << "completed!" << std::endl; });
	return 0;
}
