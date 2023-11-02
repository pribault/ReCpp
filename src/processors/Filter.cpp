export module recpp.processors.Filter;

import recpp.processors.impl.FilterPrivate;
import rscpp.Processor;

import <functional>;
import <memory>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class Filter : public Processor<T, T>
	{
	public:
		explicit Filter(const Publisher<T> &publisher, const function<bool(const T & /* value */)> &method)
			: Processor<T, T>(shared_ptr<Processor<T, T>>(new FilterPrivate<T>(*this, publisher, method)))
		{
		}
	};
} // namespace recpp
