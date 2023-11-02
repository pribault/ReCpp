export module recpp.processors.impl.FilterPrivate;

import recpp.subscriptions.FilterSubscription;
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
	class FilterPrivate : public Processor<T, T>
	{
	public:
		explicit FilterPrivate(Processor<T, T> &parent, const Publisher<T> &publisher, const function<bool(const T & /* value */)> &method)
			: m_parent(parent)
			, m_publisher(publisher)
			, m_method(method)
		{
		}

		void onSubscribe(Subscription &subscription) override
		{
			auto filterSubscription = FilterSubscription(subscription);
			m_subscriptions.push_back(filterSubscription);
			m_subscriber.onSubscribe(filterSubscription);
		}

		void onNext(const T &value) override
		{
			bool filtered = !m_method(value);
			if (!filtered)
				m_subscriber.onNext(value);
			for (auto &subscription : m_subscriptions)
				subscription.onNext(filtered);
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
			m_publisher.subscribe(m_parent);
		}

	private:
		Processor<T, T>						 &m_parent;
		Publisher<T>						  m_publisher;
		std::vector<FilterSubscription>		  m_subscriptions;
		Subscriber<T>						  m_subscriber;
		function<bool(const T & /* value */)> m_method;
	};
} // namespace recpp
