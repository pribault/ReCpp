export module recpp.processors.SubscribeOn;

import recpp.async.Scheduler;
import recpp.processors.impl.SubscribeOnPrivate;
import rscpp.Processor;

import <memory>;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class SubscribeOn : public Processor<T, T>
	{
	public:
		explicit SubscribeOn(const Publisher<T> &publisher, Scheduler &scheduler)
			: Processor<T, T>(shared_ptr<Processor<T, T>>(new SubscribeOnPrivate<T>(*this, publisher, scheduler)))
		{
		}
	};
} // namespace recpp
