export module recpp.observables.impl.DeferObservable;

import rscpp;

using namespace rscpp;

export namespace recpp
{
	template <typename T, typename F>
	class DeferObservable : public Publisher<T>
	{
	public:
		DeferObservable(F function)
			: m_function(function)
		{
		}

		void subscribe(Subscriber<T> &subscriber) override
		{
			m_function().Publisher<T>::subscribe(subscriber);
		}

	private:
		F m_function;
	};
} // namespace recpp
