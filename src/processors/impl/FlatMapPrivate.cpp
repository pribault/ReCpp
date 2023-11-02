export module recpp.processors.impl.FlatMapPrivate;

import recpp.subscribers.DefaultSubscriber;
import recpp.subscriptions.ForwardSubscription;
import rscpp.Processor;
import rscpp.Publisher;
import rscpp.Subscriber;
import rscpp.Subscription;

import <exception>;
import <functional>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T, typename R>
	class FlatMapPrivate : public Processor<T, R>
	{
	public:
		explicit FlatMapPrivate(Processor<T, R> &parent, const Publisher<T> &publisher, const function<Publisher<R>(const T & /* value */)> &method)
			: m_parent(parent)
			, m_publisher(publisher)
			, m_method(method)
		{
		}

		void onSubscribe(Subscription &subscription) override
		{
			auto forwardSubscription = ForwardSubscription(subscription);
			m_subscriber.onSubscribe(forwardSubscription);
		}

		void onNext(const T &value) override
		{
			auto result = m_method(value);
			m_runningPublishers++;
			auto subscriber = DefaultSubscriber<R>([this](const R &value) { m_subscriber.onNext(value); },
												   [this](const exception_ptr &error) { m_subscriber.onError(error); },
												   [this]()
												   {
													   m_runningPublishers--;
													   if (!m_runningPublishers && m_completed)
														   m_subscriber.onComplete();
												   });
			result.subscribe(subscriber);
		}

		void onError(const exception_ptr &error) override
		{
			m_subscriber.onError(error);
		}

		void onComplete() override
		{
			m_completed = true;
			if (!m_runningPublishers)
				m_subscriber.onComplete();
		}

		void subscribe(Subscriber<R> &subscriber) override
		{
			m_subscriber = subscriber;
			m_publisher.subscribe(m_parent);
		}

	private:
		Processor<T, R>								 &m_parent;
		Publisher<T>								  m_publisher;
		Subscriber<R>								  m_subscriber;
		function<Publisher<R>(const T & /* value */)> m_method;
		size_t										  m_runningPublishers = 0;
		bool										  m_completed = false;
	};
} // namespace recpp
