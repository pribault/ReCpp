export module recpp.processors.Filter;

import recpp.processors.impl.FilterPrivate;
import rscpp.Processor;

import <memory>;

using namespace rscpp;

export namespace recpp
{
	template <typename T, typename R>
	class Filter : public Processor<T, R>
	{
	public:
		template <typename M>
		explicit Filter(const Publisher<T> &publisher, M method)
			: Processor<T, R>(shared_ptr<Processor<T, R>>(new FilterPrivate<T, R>(publisher, method)))
		{
			static_cast<FilterPrivate<T, R> *>(Publisher<R>::d_ptr.get())->setParent(*this);
		}
	};
} // namespace recpp
