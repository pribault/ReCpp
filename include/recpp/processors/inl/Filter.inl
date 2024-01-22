#pragma once

template <typename T>
recpp::processors::Filter<T>::Filter(const rscpp::Publisher<T> &publisher, const std::function<bool(const T & /* value */)> &method)
	: rscpp::Processor<T, T>(std::shared_ptr<rscpp::Processor<T, T>>(new Impl(*this, publisher, method)))
{
}

template <typename T>
recpp::processors::Filter<T>::Impl::Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher,
										 const std::function<bool(const T & /* value */)> &method)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_method(method)
{
}

template <typename T>
void recpp::processors::Filter<T>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	auto filterSubscription = recpp::subscriptions::FilterSubscription(subscription);
	m_subscriptions.push_back(filterSubscription);
	m_subscriber.onSubscribe(filterSubscription);
}

template <typename T>
void recpp::processors::Filter<T>::Impl::onNext(const T &value)
{
	bool filtered = !m_method(value);
	if (!filtered)
		m_subscriber.onNext(value);
	for (auto &subscription : m_subscriptions)
		subscription.onNext(filtered);
}

template <typename T>
void recpp::processors::Filter<T>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
}

template <typename T>
void recpp::processors::Filter<T>::Impl::onComplete()
{
	m_subscriber.onComplete();
}

template <typename T>
void recpp::processors::Filter<T>::Impl::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
