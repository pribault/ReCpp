#pragma once

template <typename T>
recpp::subscriptions::IntervalSubscription<T>::IntervalSubscription(const rscpp::Subscriber<T> &subscriber, const recpp::async::Scheduler::TimePoint &start,
																	const recpp::async::Scheduler::Duration &period, recpp::async::Scheduler &scheduler)
	: rscpp::Subscription(std::make_shared<Impl>(subscriber, start, period, scheduler))
{
	std::static_pointer_cast<Impl>(d_ptr)->setParent(*this);
}

template <typename T>
recpp::subscriptions::IntervalSubscription<T>::Impl::Impl(const rscpp::Subscriber<T> &subscriber, const recpp::async::Scheduler::TimePoint &start,
														  const recpp::async::Scheduler::Duration &period, recpp::async::Scheduler &scheduler)
	: m_subscriber(subscriber)
	, m_start(start)
	, m_period(period)
	, m_scheduler(scheduler)
{
}

template <typename T>
void recpp::subscriptions::IntervalSubscription<T>::Impl::request(std::size_t count)
{
	m_requested += count;
	if (!m_running && !m_canceled)
		tryRequest(std::nullopt, 0);
}

template <typename T>
void recpp::subscriptions::IntervalSubscription<T>::Impl::cancel()
{
	m_canceled = true;
}

template <typename T>
void recpp::subscriptions::IntervalSubscription<T>::Impl::setParent(rscpp::Subscription &parent)
{
	m_parent = parent;
}

template <typename T>
void recpp::subscriptions::IntervalSubscription<T>::Impl::tryRequest(const std::optional<recpp::async::Scheduler::TimePoint> &prevPoint, T value)
{
	if (!m_requested)
		return;

	m_requested--;
	const auto timePoint = prevPoint.value_or(m_start);
	m_running = true;
	const auto parent = m_parent;
	m_scheduler.schedule(timePoint, {[this, parent, timePoint, value]()
									 {
										 m_running = false;
										 if (!m_canceled)
										 {
											 m_subscriber.onNext(value);
											 tryRequest(timePoint + m_period, value + 1);
										 }
									 }});
}
