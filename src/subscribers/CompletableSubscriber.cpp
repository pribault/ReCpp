#include "recpp/subscribers/CompletableSubscriber.h"

using namespace rscpp;
using namespace std;

recpp::CompletableSubscriber::CompletableSubscriber(const Subscriber<int> &subscriber)
	: m_subscriber(subscriber)
{
}

void recpp::CompletableSubscriber::onError(const exception_ptr &error)
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
