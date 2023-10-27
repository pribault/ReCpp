export module recpp.subscriptions.Subscription;

import rscpp.Subscriber;
import rscpp.Subscription;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class Subscription : public rscpp::Subscription
	{
	public:
		Subscription(const Subscriber<T> &subscriber, const typename rscpp::Subscription::RequestMethod &requestMethod,
					 const typename rscpp::Subscription::CancelMethod &cancelMethod, const rscpp::Subscription::StatePtr &state = nullptr)
			: rscpp::Subscription(requestMethod, cancelMethod, state)
			, m_subscriber(subscriber)
		{
		}

	protected:
		Subscriber<T> m_subscriber;
	};
} // namespace recpp
