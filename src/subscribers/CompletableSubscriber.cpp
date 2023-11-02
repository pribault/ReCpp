export module recpp.subscribers.CompletableSubscriber;

import rscpp.Subscriber;

import <exception>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class CompletableSubscriber
	{
	public:
		CompletableSubscriber(const Subscriber<T> &subscriber)
			: m_subscriber(subscriber)
		{
		}

		void onError(const exception_ptr &error)
		{
			if (!m_ended)
			{
				m_ended = true;
				m_subscriber.onError(error);
			}
		}

		void onComplete()
		{
			if (!m_ended)
			{
				m_ended = true;
				m_subscriber.onComplete();
			}
		}

	private:
		Subscriber<T> m_subscriber;
		bool		  m_ended = false;
	};
} // namespace recpp
