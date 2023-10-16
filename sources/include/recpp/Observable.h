#pragma once

#include <rscpp/Publisher.h>
#include <rscpp/Subscriber.h>

namespace recpp
{
	template <typename T>
	class Observable : public rscpp::Publisher<T>
	{
	public:
		template <typename Function>
		static Observable<T> create(Function function);
		template <typename Function>
		static Observable<T> defer(Function function);
		static Observable<T> empty();
		template <typename Exception>
		static Observable<T> error(Exception &&exception);
		static Observable<T> just(const T &value);
		static Observable<T> never();

		template <typename OnNext, typename OnError, typename OnComplete>
		void subscribe(OnNext onNext, OnError onError, OnComplete onComplete);

	protected:
		Observable(const typename rscpp::Publisher<T>::SubscribeMethod &subscribeMethod);
	};
} // namespace recpp

#include <recpp/Observable.inl>
