#pragma once

template <typename T>
recpp::DefaultSubscriber<T>::DefaultSubscriber(const typename rscpp::Subscriber<T>::OnNextMethod	 &onNextMethod,
											   const typename rscpp::Subscriber<T>::OnErrorMethod	 &onErrorMethod,
											   const typename rscpp::Subscriber<T>::OnCompleteMethod &onCompleteMethod)
	: recpp::Subscriber<T>(
		  [this](const auto &subscription)
		  {
			  auto state = static_cast<State *>(rscpp::Subscriber<T>::m_state.get());
			  state->subscription = subscription;
			  subscription.request(1);
		  },
		  [this, onNextMethod](const T &value)
		  {
			  auto state = static_cast<State *>(rscpp::Subscriber<T>::m_state.get());
			  onNextMethod(value);
			  state->subscription.request(1);
		  },
		  onErrorMethod, onCompleteMethod, rscpp::Subscriber<T>::StatePtr(new State))
{
}
