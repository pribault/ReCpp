export module recpp.processors.Tap;

import recpp.processors.impl.TapPrivate;
import rscpp.Processor;

import <functional>;
import <memory>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class Tap : public Processor<T, T>
	{
	public:
		using OnNextMethod = function<void(const T & /* value */)>;
		using OnErrorMethod = function<void(const exception_ptr & /* error */)>;
		using OnCompleteMethod = function<void()>;

		explicit Tap(const Publisher<T> &publisher, const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod,
					 const OnCompleteMethod &onCompleteMethod)
			: Processor<T, T>(shared_ptr<Processor<T, T>>(new TapPrivate<T>(*this, publisher, onNextMethod, onErrorMethod, onCompleteMethod)))
		{
		}
	};
} // namespace recpp
