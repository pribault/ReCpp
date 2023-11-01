export module recpp.publishers.impl.CreatePublisher;

import rscpp;

import <functional>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T, typename S>
	class CreatePublisher : public Publisher<T>
	{
	public:
		using CreateMethod = function<void(S & /* subscriber */)>;

		CreatePublisher(const CreateMethod &method)
			: m_method(method)
		{
		}

		void subscribe(Subscriber<T> &subscriber) override
		{
			auto s = S(subscriber);
			m_method(s);
		}

	private:
		CreateMethod m_method;
	};
} // namespace recpp
