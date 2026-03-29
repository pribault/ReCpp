#pragma once

template <typename T>
recpp::processors::Take<T>::Take(const rscpp::Publisher<T> &publisher, std::size_t count)
	: rscpp::Processor<T, T>(std::make_shared<Impl>(*this, publisher, count))
{
}

template <typename T>
recpp::processors::Take<T>::Impl::Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, std::size_t count)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_count(count)
{
}

template <typename T>
void recpp::processors::Take<T>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	auto filterSubscription = recpp::subscriptions::FilterSubscription(subscription);
	m_subscriptions.push_back(filterSubscription);
	m_subscriber.onSubscribe(filterSubscription);
}

template <typename T>
void recpp::processors::Take<T>::Impl::onNext(const T &value)
{
	if (m_count == 0)
		return;

	m_count--;
	m_subscriber.onNext(value);
	for (auto &subscription : m_subscriptions)
		subscription.onNext(false);
	if (m_count == 0)
	{
		for (auto &subscription : m_subscriptions)
			subscription.cancel();
		m_subscriber.onComplete();
		m_subscriptions = {};
	}
}

template <typename T>
void recpp::processors::Take<T>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
	m_subscriptions = {};
}

template <typename T>
void recpp::processors::Take<T>::Impl::onComplete()
{
	m_subscriber.onComplete();
	m_subscriptions = {};
}

template <typename T>
void recpp::processors::Take<T>::Impl::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
