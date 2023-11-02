export module recpp.publishers.impl.DeferPublisher;

import rscpp;

import <functional>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T, typename P>
	class DeferPublisher : public Publisher<T>
	{
	public:
		DeferPublisher(const function<P()> &function)
			: m_function(function)
		{
		}

		void subscribe(Subscriber<T> &subscriber) override
		{
			m_function().Publisher<T>::subscribe(subscriber);
		}

	private:
		function<P()> m_function;
	};
} // namespace recpp
