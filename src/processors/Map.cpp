export module recpp.processors.Map;

import recpp.processors.impl.MapPrivate;
import rscpp.Processor;

import <functional>;
import <memory>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T, typename R>
	class Map : public Processor<T, R>
	{
	public:
		explicit Map(const Publisher<T> &publisher, const function<R(const T & /* value */)> &method)
			: Processor<T, R>(shared_ptr<Processor<T, R>>(new MapPrivate<T, R>(*this, publisher, method)))
		{
		}
	};
} // namespace recpp
