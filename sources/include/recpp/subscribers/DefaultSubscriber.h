#pragma once

#include <recpp/Subscriber.h>

namespace recpp
{
	template <typename T>
	class DefaultSubscriber : public recpp::Subscriber<T>
	{
	public:
		DefaultSubscriber(const typename rscpp::Subscriber<T>::OnNextMethod &onNextMethod, const typename rscpp::Subscriber<T>::OnErrorMethod &onErrorMethod,
						  const typename rscpp::Subscriber<T>::OnCompleteMethod &onCompleteMethod);

	protected:
		struct State : public rscpp::Subscriber<T>::State
		{
			rscpp::Subscription subscription;
		};
	};
} // namespace recpp

#include <recpp/subscribers/DefaultSubscriber.inl>
