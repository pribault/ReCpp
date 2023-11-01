export module recpp.processors.Map;

import recpp.processors.impl.MapPrivate;
import rscpp.Processor;

import <memory>;

using namespace rscpp;

export namespace recpp
{
	template <typename T, typename R>
	class Map : public Processor<T, R>
	{
	public:
		template <typename M>
		explicit Map(const Publisher<T> &publisher, M method)
			: Processor<T, R>(shared_ptr<Processor<T, R>>(new MapPrivate<T, R>(*this, publisher, method)))
		{
		}
	};
} // namespace recpp
