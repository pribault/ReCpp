#pragma once

template <typename T>
recpp::DefaultSubscriber<T>::DefaultSubscriber(const typename rscpp::Subscriber<T>::OnNextMethod	 &onNextMethod,
											   const typename rscpp::Subscriber<T>::OnErrorMethod	 &onErrorMethod,
											   const typename rscpp::Subscriber<T>::OnCompleteMethod &onCompleteMethod)
	: Subscriber<T>(std::bind(&DefaultSubscriber<T>::internalOnSubscribe, this, std::placeholders::_1),
					std::bind(&DefaultSubscriber<T>::internalOnNext, this, std::placeholders::_1), onErrorMethod, onCompleteMethod,
					rscpp::Subscriber<T>::StatePtr(new State))
	, m_onNext(onNextMethod)
{
}

template <typename T>
void recpp::DefaultSubscriber<T>::internalOnSubscribe(const rscpp::Subscription &subscription)
{
	auto state = static_cast<State *>(rscpp::Subscriber<T>::m_state.get());
	state->subscription = subscription;
	subscription.request(1);
}

template <typename T>
void recpp::DefaultSubscriber<T>::internalOnNext(const T &value)
{
	auto state = static_cast<State *>(rscpp::Subscriber<T>::m_state.get());
	m_onNext(value);
	state->subscription.request(1);
}
