export module recpp.subscriptions.EmptySubscription;

import recpp.subscriptions.Subscription;
import rscpp.Subscriber;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class EmptySubscription : public recpp::Subscription<T>
	{
	public:
		EmptySubscription(const Subscriber<T> &subscriber)
			: Subscription<T>(subscriber, std::bind(&EmptySubscription<T>::internalRequest, this, std::placeholders::_1), nullptr)
		{
		}

	private:
		void internalRequest(size_t count) noexcept { Subscription<T>::m_subscriber.onComplete(); }
	};
} // namespace recpp
