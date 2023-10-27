export module recpp.subscriptions.RangeSubscription;

import recpp.subscriptions.Subscription;
import rscpp.Subscriber;

import <iterator>;

using namespace rscpp;

export namespace recpp
{
	template <typename T, class I>
	class RangeSubscription : public recpp::Subscription<T>
	{
	public:
		RangeSubscription(const Subscriber<T> &subscriber, I first, I last)
			: Subscription<T>(subscriber, std::bind(&RangeSubscription<T, I>::internalRequest, this, std::placeholders::_1), nullptr,
							  rscpp::Subscription::StatePtr(new State({.current = first, .last = last})))
		{
		}

	protected:
		struct State : public rscpp::Subscription::State
		{
			I current;
			I last;
		};

	private:
		void internalRequest(size_t count) noexcept
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
	};
} // namespace recpp
