export module recpp.processors.ObserveOn;

import recpp.async.Scheduler;
import recpp.processors.impl.ObserveOnPrivate;
import rscpp.Processor;

import <memory>;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class ObserveOn : public Processor<T, T>
	{
	public:
		explicit ObserveOn(const Publisher<T> &publisher, Scheduler &scheduler)
			: Processor<T, T>(shared_ptr<Processor<T, T>>(new ObserveOnPrivate<T>(*this, publisher, scheduler)))
		{
		}
	};
} // namespace recpp
