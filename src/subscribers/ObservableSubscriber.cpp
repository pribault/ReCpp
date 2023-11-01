export module recpp.subscribers.ObservableSubscriber;

import rscpp.Subscriber;

import <exception>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class ObservableSubscriber
	{
	public:
		ObservableSubscriber(const Subscriber<T> &subscriber)
			: m_subscriber(subscriber)
		{
		}

		void onNext(const T &value)
		{
			m_subscriber.onNext(value);
		}

		void onError(const exception_ptr &error)
		{
			m_subscriber.onError(error);
		}

		void onComplete()
		{
			m_subscriber.onComplete();
		}

	private:
		Subscriber<T> m_subscriber;
	};
} // namespace recpp
