export module recpp.processors.impl.AndThenPrivate;

import rscpp.Processor;
import rscpp.Publisher;
import rscpp.Subscriber;
import rscpp.Subscription;

import <exception>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T, typename R>
	class AndThenPrivate : public Processor<T, R>
	{
	public:
		explicit AndThenPrivate(Processor<T, R> &parent, const Publisher<T> &source, const Publisher<R> &dest)
			: m_parent(parent)
			, m_source(source)
			, m_dest(dest)
		{
		}

		void onSubscribe(Subscription &subscription) override
		{
			m_subscription = subscription;
			m_remaining = numeric_limits<size_t>::max();
			m_subscription.request(m_remaining);
		}

		void onNext(const T &) override
		{
			m_remaining--;
			if (!m_remaining)
			{
				m_remaining = numeric_limits<size_t>::max();
				m_subscription.request(m_remaining);
			}
		}

		void onError(const exception_ptr &error) override
		{
			m_subscriber.onError(error);
		}

		void onComplete() override
		{
			m_dest.subscribe(m_subscriber);
		}

		void subscribe(Subscriber<R> &subscriber) override
		{
			m_subscriber = subscriber;
			m_source.subscribe(m_parent);
		}

	private:
		Processor<T, R> &m_parent;
		Publisher<T>	 m_source;
		Publisher<R>	 m_dest;
		Subscriber<R>	 m_subscriber;
		Subscription	 m_subscription;
		size_t			 m_remaining = 0;
	};
} // namespace recpp
