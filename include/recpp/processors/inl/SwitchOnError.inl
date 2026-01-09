#pragma once

template <typename T>
recpp::processors::SwitchOnError<T>::SwitchOnError(const rscpp::Publisher<T> &publisher, const rscpp::Publisher<T> &fallbackPublisher)
	: rscpp::Processor<T, T>(std::make_shared<Impl>(*this, publisher, fallbackPublisher))
{
}

template <typename T>
recpp::processors::SwitchOnError<T>::Impl::Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher,
												const rscpp::Publisher<T> &fallbackPublisher)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_fallbackPublisher(fallbackPublisher)
{
}

template <typename T>
void recpp::processors::SwitchOnError<T>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	auto forwardSubscription = recpp::subscriptions::ForwardSubscription(subscription);
	m_subscriber.onSubscribe(forwardSubscription);
}

template <typename T>
void recpp::processors::SwitchOnError<T>::Impl::onNext(const T &value)
{
	m_subscriber.onNext(value);
}

template <typename T>
void recpp::processors::SwitchOnError<T>::Impl::onError(const std::exception_ptr &error)
{
	if (!m_publisherErrored)
	{
		m_publisherErrored = true;
		m_fallbackPublisher.subscribe(m_parent);
	}
	else
		m_subscriber.onError(error);
}

template <typename T>
void recpp::processors::SwitchOnError<T>::Impl::onComplete()
{
	m_subscriber.onComplete();
}

template <typename T>
void recpp::processors::SwitchOnError<T>::Impl::subscribe(rscpp::Subscriber<T> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
	m_publisherErrored = false;
}
