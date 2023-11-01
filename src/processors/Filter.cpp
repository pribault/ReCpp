export module recpp.processors.Filter;

import recpp.processors.impl.FilterPrivate;
import rscpp.Processor;

import <memory>;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class Filter : public Processor<T, T>
	{
	public:
		template <typename M>
		explicit Filter(const Publisher<T> &publisher, M method)
			: Processor<T, T>(shared_ptr<Processor<T, T>>(new FilterPrivate<T>(*this, publisher, method)))
		{
		}
	};
} // namespace recpp
