export module recpp.processors.impl.ObserveOnPrivate;

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
	class ObserveOnPrivate : public Processor<T, T>
	{
	public:
		explicit ObserveOnPrivate(Processor<T, T> &parent, const Publisher<T> &publisher, Scheduler &scheduler)
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
			auto subscriber = m_subscriber;
			m_scheduler.schedule(Schedulable([subscriber, value]() mutable { subscriber.onNext(value); }));
		}

		void onError(const exception_ptr &error) override
		{
			auto subscriber = m_subscriber;
			m_scheduler.schedule(Schedulable([subscriber, error]() mutable { subscriber.onError(error); }));
		}

		void onComplete() override
		{
			auto subscriber = m_subscriber;
			m_scheduler.schedule(Schedulable([subscriber]() mutable { subscriber.onComplete(); }));
		}

		void subscribe(Subscriber<T> &subscriber) override
		{
			m_subscriber = subscriber;
			m_publisher.subscribe(m_parent);
		}

	private:
		Processor<T, T> &m_parent;
		Publisher<T>	 m_publisher;
		Subscriber<T>	 m_subscriber;
		Scheduler		&m_scheduler;
	};
} // namespace recpp
