export module recpp.processors.FlatMap;

import recpp.processors.impl.FlatMapPrivate;
import rscpp.Processor;

import <memory>;

using namespace rscpp;

export namespace recpp
{
	template <typename T, typename R>
	class FlatMap : public Processor<T, R>
	{
	public:
		template <typename M>
		explicit FlatMap(const Publisher<T> &publisher, M method)
			: Processor<T, R>(shared_ptr<Processor<T, R>>(new FlatMapPrivate<T, R>(*this, publisher, method)))
		{
		}
	};
} // namespace recpp
