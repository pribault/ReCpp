#pragma once

#include <recpp/Subscription.h>

#include <iterator>

namespace recpp
{
	template <typename T, class I>
	class RangeSubscription : public recpp::Subscription<T>
	{
	public:
		RangeSubscription(const rscpp::Subscriber<T> &subscriber, I first, I last);

	protected:
		struct State : public rscpp::Subscription::State
		{
			State(I first, I last)
				: current(first)
				, last(last)
			{
			}
			I current;
			I last;
		};

	private:
		void internalRequest(size_t count) noexcept;
	};
} // namespace recpp

#include <recpp/subscriptions/RangeSubscription.inl>
