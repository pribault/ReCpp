export module recpp.processors.Tap;

import recpp.processors.impl.TapPrivate;
import rscpp.Processor;

import <memory>;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class Tap : public Processor<T, T>
	{
	public:
		template <typename N, typename E, typename C>
		explicit Tap(const Publisher<T> &publisher, N onNextMethod, E onErrorMethod, C onCompleteMethod)
			: Processor<T, T>(shared_ptr<Processor<T, T>>(new TapPrivate<T>(publisher, onNextMethod, onErrorMethod, onCompleteMethod)))
		{
			static_cast<TapPrivate<T> *>(Publisher<T>::d_ptr.get())->setParent(*this);
		}
	};
} // namespace recpp
