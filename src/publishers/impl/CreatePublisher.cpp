export module recpp.publishers.impl.CreatePublisher;

import rscpp;

using namespace rscpp;

export namespace recpp
{
	template <typename T, typename F>
	class CreatePublisher : public Publisher<T>
	{
	public:
		CreatePublisher(F function)
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
