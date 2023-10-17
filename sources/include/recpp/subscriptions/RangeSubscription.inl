#pragma once

template <typename T, class I>
recpp::RangeSubscription<T, I>::RangeSubscription(const rscpp::Subscriber<T> &subscriber, I first, I last)
	: Subscription<T>(subscriber, std::bind(&RangeSubscription<T, I>::internalRequest, this, std::placeholders::_1), nullptr,
					  rscpp::Subscription::StatePtr(new State(first, last)))
{
}

template <typename T, class I>
void recpp::RangeSubscription<T, I>::internalRequest(size_t count) noexcept
{
	auto state = static_cast<State *>(rscpp::Subscription::m_state.get());
	for (size_t i = 0; i < count; i++)
	{
		if (state->current == state->last)
		{
			Subscription<T>::m_subscriber.onComplete();
			break;
		}
		const auto it = state->current++;
		Subscription<T>::m_subscriber.onNext(*it);
	}
}
