#pragma once

template <typename T>
recpp::processors::TakeWhile<T>::TakeWhile(const rscpp::Publisher<T> &publisher, const std::function<bool(const T & /* value */)> &predicate)
	: rscpp::Processor<T, T>(std::make_shared<Impl>(*this, publisher, predicate))
{
}

template <typename T>
recpp::processors::TakeWhile<T>::Impl::Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher,
											const std::function<bool(const T & /* value */)> &predicate)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_predicate(predicate)
{
}

template <typename T>
void recpp::processors::TakeWhile<T>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	auto filterSubscription = recpp::subscriptions::FilterSubscription(subscription);
	m_subscriptions.push_back(filterSubscription);
	m_subscriber.onSubscribe(filterSubscription);
}

template <typename T>
void recpp::processors::TakeWhile<T>::Impl::onNext(const T &value)
{
	bool stop = !m_predicate(value);
	if (!stop)
		m_subscriber.onNext(value);
	for (auto &subscription : m_subscriptions)
		subscription.onNext(stop);
	if (stop)
	{
		for (auto &subscription : m_subscriptions)
			subscription.cancel();
		m_subscriber.onComplete();
	}
}

template <typename T>
void recpp::processors::TakeWhile<T>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
}

template <typename T>
void recpp::processors::TakeWhile<T>::Impl::onComplete()
{
	m_subscriber.onComplete();
}

template <typename T>
void recpp::processors::TakeWhile<T>::Impl::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
