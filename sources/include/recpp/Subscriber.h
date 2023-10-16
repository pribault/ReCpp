#pragma once

#include <rscpp/Subscriber.h>

namespace recpp
{
	template <typename T>
	class Subscriber : public rscpp::Subscriber<T>
	{
	public:
		Subscriber(const typename rscpp::Subscriber<T>::OnSubscribeMethod &onSubscribeMethod, const typename rscpp::Subscriber<T>::OnNextMethod &onNextMethod,
				   const typename rscpp::Subscriber<T>::OnErrorMethod &onErrorMethod, const typename rscpp::Subscriber<T>::OnCompleteMethod &onCompleteMethod);
	};
} // namespace recpp

#include <recpp/Subscriber.inl>
