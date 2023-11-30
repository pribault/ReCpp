#include "recpp/subscribers/CompletableSubscriber.h"

using namespace rscpp;
using namespace std;

recpp::subscribers::CompletableSubscriber::CompletableSubscriber(const Subscriber<int> &subscriber)
	: m_subscriber(subscriber)
{
}

void recpp::subscribers::CompletableSubscriber::onError(const exception_ptr &error)
{
	if (!m_ended)
	{
		m_ended = true;
		m_subscriber.onError(error);
	}
}

void recpp::subscribers::CompletableSubscriber::onComplete()
{
	if (!m_ended)
	{
		m_ended = true;
		m_subscriber.onComplete();
	}
}
