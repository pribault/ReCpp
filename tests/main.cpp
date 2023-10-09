#include <ReCpp/Observable.h>

#include <iostream>

using namespace recpp;

int main()
{
	//Observable<int>::error(std::make_exception_ptr(std::runtime_error("test")))
	Observable<int>::just(42)
		.subscribe([](int value)
		{
			std::cout << "value=" << value << std::endl;
		},
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
		[]()
		{
			std::cout << "completed!" << std::endl;
		});
	return 0;
}
