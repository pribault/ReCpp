export module recpp.processors.impl.TapPrivate;

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
	template <typename T>
	class TapPrivate : public Processor<T, T>
	{
	public:
		template <typename N, typename E, typename C>
		explicit TapPrivate(const Publisher<T> &publisher, N onNextMethod, E onErrorMethod, C onCompleteMethod)
			: m_publisher(publisher)
			, m_onNextMethod(onNextMethod)
			, m_onErrorMethod(onErrorMethod)
			, m_onCompleteMethod(onCompleteMethod)
		{
		}

		void onSubscribe(Subscription &subscription) override
		{
			auto forwardSubscription = ForwardSubscription(subscription);
			m_subscriber.onSubscribe(forwardSubscription);
		}

		void onNext(const T &value) override
		{
			if (m_onNextMethod)
				m_onNextMethod(value);
			m_subscriber.onNext(value);
		}

		void onError(const exception_ptr &error) override
		{
			if (m_onErrorMethod)
				m_onErrorMethod(error);
			m_subscriber.onError(error);
		}

		void onComplete() override
		{
			if (m_onCompleteMethod)
				m_onCompleteMethod();
			m_subscriber.onComplete();
		}

		void subscribe(Subscriber<T> &subscriber) override
		{
			m_subscriber = subscriber;
			m_publisher.subscribe(m_parent);
		}

		void setParent(const Processor<T, T> &parent)
		{
			m_parent = parent;
		}

	private:
		Processor<T, T>									  m_parent;
		Publisher<T>									  m_publisher;
		Subscriber<T>									  m_subscriber;
		function<void(const T & /* value */)>			  m_onNextMethod;
		function<void(const exception_ptr & /* error */)> m_onErrorMethod;
		function<void()>								  m_onCompleteMethod;
	};
} // namespace recpp
