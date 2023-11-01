export module recpp.processors.impl.SubscribeOnPrivate;

import recpp.async.Schedulable;
import recpp.async.Scheduler;
import recpp.subscriptions.ForwardSubscription;
import rscpp.Processor;
import rscpp.Publisher;
import rscpp.Subscriber;
import rscpp.Subscription;

import <exception>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class SubscribeOnPrivate : public Processor<T, T>
	{
	public:
		explicit SubscribeOnPrivate(Processor<T, T> &parent, const Publisher<T> &publisher, Scheduler &scheduler)
			: m_parent(parent)
			, m_publisher(publisher)
			, m_scheduler(scheduler)
		{
		}

		void onSubscribe(Subscription &subscription) override
		{
			auto forwardSubscription = ForwardSubscription(subscription);
			m_subscriber.onSubscribe(forwardSubscription);
		}

		void onNext(const T &value) override
		{
			m_subscriber.onNext(value);
		}

		void onError(const exception_ptr &error) override
		{
			m_subscriber.onError(error);
		}

		void onComplete() override
		{
			m_subscriber.onComplete();
		}

		void subscribe(Subscriber<T> &subscriber) override
		{
			m_subscriber = subscriber;
			auto publisher = m_publisher;
			auto parent = m_parent;
			m_scheduler.schedule(Schedulable([publisher, parent]() mutable { publisher.subscribe(parent); }));
		}

	private:
		Processor<T, T> &m_parent;
		Publisher<T>	 m_publisher;
		Subscriber<T>	 m_subscriber;
		Scheduler		&m_scheduler;
	};
} // namespace recpp
