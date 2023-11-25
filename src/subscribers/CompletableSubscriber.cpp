#include "recpp/subscribers/CompletableSubscriber.h"

recpp::CompletableSubscriber::CompletableSubscriber(const rscpp::Subscriber<int> &subscriber)
	: m_subscriber(subscriber)
{
}

void recpp::CompletableSubscriber::onError(const std::exception_ptr &error)
{
	if (!m_ended)
	{
		m_ended = true;
		m_subscriber.onError(error);
	}
}

void recpp::CompletableSubscriber::onComplete()
{
	if (!m_ended)
	{
		m_ended = true;
		m_subscriber.onComplete();
	}
}
