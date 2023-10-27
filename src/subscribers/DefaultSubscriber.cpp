export module recpp.subscribers.DefaultSubscriber;

import recpp.subscribers.Subscriber;
import rscpp.Subscription;

using namespace rscpp;

export namespace recpp
{
	template <typename T>
	class DefaultSubscriber : public recpp::Subscriber<T>
	{
	public:
		DefaultSubscriber(const typename rscpp::Subscriber<T>::OnNextMethod &onNextMethod, const typename rscpp::Subscriber<T>::OnErrorMethod &onErrorMethod,
						  const typename rscpp::Subscriber<T>::OnCompleteMethod &onCompleteMethod)
			: Subscriber<T>(std::bind(&DefaultSubscriber<T>::internalOnSubscribe, this, std::placeholders::_1),
							std::bind(&DefaultSubscriber<T>::internalOnNext, this, std::placeholders::_1), onErrorMethod, onCompleteMethod,
							rscpp::Subscriber<T>::StatePtr(new State()))
			, m_onNext(onNextMethod)
		{
		}

	protected:
		struct State : public rscpp::Subscriber<T>::State
		{
			Subscription subscription;
		};

	private:
		inline void internalOnSubscribe(const Subscription &subscription)
		{
			auto state = static_cast<State *>(rscpp::Subscriber<T>::m_state.get());
			state->subscription = subscription;
			subscription.request(1);
		}

		inline void internalOnNext(const T &value)
		{
			auto state = static_cast<State *>(rscpp::Subscriber<T>::m_state.get());
			m_onNext(value);
			state->subscription.request(1);
		}

		typename rscpp::Subscriber<T>::OnNextMethod m_onNext;
	};
} // namespace recpp
