export module recpp.observables.impl.CreateObservable;

import rscpp;

using namespace rscpp;

export namespace recpp
{
	template <typename T, typename F>
	class CreateObservable : public Publisher<T>
	{
	public:
		CreateObservable(F function)
			: m_function(function)
		{
		}

		void subscribe(Subscriber<T> &subscriber) override
		{
			m_function(subscriber);
		}

	private:
		F m_function;
	};
} // namespace recpp
