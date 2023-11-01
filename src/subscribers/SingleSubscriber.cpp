export module recpp.subscribers.SingleSubscriber;

import rscpp.Subscriber;

import <exception>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class SingleSubscriber
	{
	public:
		SingleSubscriber(const Subscriber<T> &subscriber)
			: m_subscriber(subscriber)
		{
		}

		void onNext(const T &value)
		{
			m_subscriber.onNext(value);
			m_subscriber.onComplete();
		}

		void onError(const exception_ptr &error)
		{
			m_subscriber.onError(error);
		}

	private:
		Subscriber<T> m_subscriber;
	};
} // namespace recpp
