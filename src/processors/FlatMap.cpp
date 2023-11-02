export module recpp.processors.FlatMap;

import recpp.processors.impl.FlatMapPrivate;
import rscpp.Processor;

import <functional>;
import <memory>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T, typename R>
	class FlatMap : public Processor<T, R>
	{
	public:
		explicit FlatMap(const Publisher<T> &publisher, const function<Publisher<R>(const T & /* value */)> &method)
			: Processor<T, R>(shared_ptr<Processor<T, R>>(new FlatMapPrivate<T, R>(*this, publisher, method)))
		{
		}
	};
} // namespace recpp
