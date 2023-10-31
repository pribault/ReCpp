export module recpp.publishers.impl.ErrorPublisher;

import recpp.subscriptions.EmptySubscription;
import rscpp;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class ErrorPublisher : public Publisher<T>
	{
	public:
		ErrorPublisher(const exception_ptr &error)
			: m_error(error)
		{
		}

		void subscribe(Subscriber<T> &subscriber) override
		{
			subscriber.onError(m_error);
		}

	private:
		exception_ptr m_error;
	};
} // namespace recpp
