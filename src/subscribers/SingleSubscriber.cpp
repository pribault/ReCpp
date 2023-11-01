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
			if (!m_ended)
			{
				m_ended = true;
				m_subscriber.onNext(value);
				m_subscriber.onComplete();
			}
		}

		void onError(const exception_ptr &error)
		{
			if (!m_ended)
			{
				m_ended = true;
				m_subscriber.onError(error);
			}
		}

	private:
		Subscriber<T> m_subscriber;
		bool		  m_ended = false;
	};
} // namespace recpp
