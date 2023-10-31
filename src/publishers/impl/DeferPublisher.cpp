export module recpp.publishers.impl.DeferPublisher;

import rscpp;

using namespace rscpp;

export namespace recpp
{
	template <typename T, typename F>
	class DeferPublisher : public Publisher<T>
	{
	public:
		DeferPublisher(F function)
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
