export module recpp.processors.impl.FilterPrivate;

import recpp.subscriptions.ForwardSubscription;
import rscpp.Processor;
import rscpp.Publisher;
import rscpp.Subscriber;
import rscpp.Subscription;

import <exception>;
import <functional>;
import <memory>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T, typename R>
	class FilterPrivate : public Processor<T, R>
	{
	public:
		template <typename M>
		explicit FilterPrivate(const Publisher<T> &publisher, M method)
			: m_publisher(publisher)
			, m_method(method)
		{
		}

		void onSubscribe(Subscription &subscription) override
		{
			m_subscription = subscription;
			auto forwardSubscription = ForwardSubscription(subscription);
			m_subscriber.onSubscribe(forwardSubscription);
		}

		void onNext(const T &value) override
		{
			if (m_method(value))
				m_subscriber.onNext(value);
			m_subscription.request(1);
		}

		void onError(const exception_ptr &error) override
		{
			m_subscriber.onError(error);
		}

		void onComplete() override
		{
			m_subscriber.onComplete();
		}

		void subscribe(Subscriber<R> &subscriber) override
		{
			m_subscriber = subscriber;
			m_publisher.subscribe(m_parent);
		}

		void setParent(const Processor<T, R> &parent)
		{
			m_parent = parent;
		}

	private:
		Processor<T, R>					   m_parent;
		Publisher<T>					   m_publisher;
		Subscription					   m_subscription;
		Subscriber<R>					   m_subscriber;
		function<R(const T & /* value */)> m_method;
	};
} // namespace recpp
