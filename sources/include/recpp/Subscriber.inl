#pragma once

template <typename T>
recpp::Subscriber<T>::Subscriber(const typename rscpp::Subscriber<T>::OnSubscribeMethod &onSubscribeMethod,
								 const typename rscpp::Subscriber<T>::OnNextMethod		&onNextMethod,
								 const typename rscpp::Subscriber<T>::OnErrorMethod		&onErrorMethod,
								 const typename rscpp::Subscriber<T>::OnCompleteMethod	&onCompleteMethod)
	: rscpp::Subscriber<T>(onSubscribeMethod, onNextMethod, onErrorMethod, onCompleteMethod)
{
}
