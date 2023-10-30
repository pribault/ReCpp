export module recpp.observables.impl.ProcessedObservable;

import rscpp;

using namespace rscpp;

export namespace recpp
{
	template <typename T, typename R>
	class ProcessedObservable : public Publisher<R>
	{
	public:
		ProcessedObservable(const Processor<T, R> &processor)
			: m_processor(processor)
		{
		}

		void subscribe(Subscriber<R> &subscriber) override
		{
			m_processor.subscribe(subscriber);
		}

	private:
		Processor<T, R> m_processor;
	};
} // namespace recpp
