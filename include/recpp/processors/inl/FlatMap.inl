#pragma once

#include <recpp/subscribers/DefaultSubscriber.h>
#include <recpp/subscriptions/ForwardSubscription.h>

template <typename T, typename R>
recpp::FlatMap<T, R>::FlatMap(const rscpp::Publisher<T> &publisher, const std::function<rscpp::Publisher<R>(const T & /* value */)> &method)
	: rscpp::Processor<T, R>(std::shared_ptr<rscpp::Processor<T, R>>(new Impl(*this, publisher, method)))
{
}

template <typename T, typename R>
recpp::FlatMap<T, R>::Impl::Impl(rscpp::Processor<T, R> &parent, const rscpp::Publisher<T> &publisher,
								 const std::function<rscpp::Publisher<R>(const T & /* value */)> &method)
	: m_parent(parent)
	, m_publisher(publisher)
	, m_method(method)
{
}

template <typename T, typename R>
void recpp::FlatMap<T, R>::Impl::onSubscribe(rscpp::Subscription &subscription)
{
	auto forwardSubscription = ForwardSubscription(subscription);
	m_subscriber.onSubscribe(forwardSubscription);
}

template <typename T, typename R>
void recpp::FlatMap<T, R>::Impl::onNext(const T &value)
{
	auto result = m_method(value);
	m_runningPublishers++;
	auto subscriber =
		DefaultSubscriber<R>([this](const R &value) { m_subscriber.onNext(value); }, [this](const exception_ptr &error) { m_subscriber.onError(error); },
							 [this]()
							 {
								 m_runningPublishers--;
								 if (!m_runningPublishers && m_completed)
									 m_subscriber.onComplete();
							 });
	result.subscribe(subscriber);
}

template <typename T, typename R>
void recpp::FlatMap<T, R>::Impl::onError(const std::exception_ptr &error)
{
	m_subscriber.onError(error);
}

template <typename T, typename R>
void recpp::FlatMap<T, R>::Impl::onComplete()
{
	m_completed = true;
	if (!m_runningPublishers)
		m_subscriber.onComplete();
}

template <typename T, typename R>
void recpp::FlatMap<T, R>::Impl::subscribe(rscpp::Subscriber<R> &subscriber)
{
	m_subscriber = subscriber;
	m_publisher.subscribe(m_parent);
}
