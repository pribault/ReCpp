export module recpp.processors.AndThen;

import recpp.processors.impl.AndThenPrivate;
import rscpp.Processor;

using namespace rscpp;

export namespace recpp
{
	template <typename T, typename R>
	class AndThen : public Processor<T, R>
	{
	public:
		explicit AndThen(const Publisher<T> &source, const Publisher<R> &dest)
			: Processor<T, R>(shared_ptr<Processor<T, R>>(new AndThenPrivate<T, R>(*this, source, dest)))
		{
		}
	};
} // namespace recpp
