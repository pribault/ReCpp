#pragma once

template <typename T, class I>
recpp::RangeSubscription<T, I>::RangeSubscription(const rscpp::Subscriber<T> &subscriber, I first, I last)
	: rscpp::Subscription(std::shared_ptr<rscpp::Subscription>(new Impl(subscriber, first, last)))
{
}

template <typename T, class I>
recpp::RangeSubscription<T, I>::Impl::Impl(const rscpp::Subscriber<T> &subscriber, I first, I last)
	: m_subscriber(subscriber)
	, m_current(first)
	, m_last(last)
{
}

template <typename T, class I>
void recpp::RangeSubscription<T, I>::Impl::request(size_t count)
{
	for (size_t i = 0; i < count; i++)
	{
		if (m_completed || m_canceled)
			break;
		if (m_current == m_last)
		{
			m_completed = true;
			m_subscriber.onComplete();
			break;
		}
		const auto it = m_current++;
		m_subscriber.onNext(*it);
	}
}

template <typename T, class I>
void recpp::RangeSubscription<T, I>::Impl::cancel()
{
	m_canceled = true;
	m_subscriber.onComplete();
}
