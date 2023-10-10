#pragma once

#include <rscpp/Publisher.h>
#include <rscpp/Subscriber.h>

#include <functional>

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

		void subscribe(rscpp::Subscriber<T> &subscriber) noexcept override;
		template <typename OnNext, typename OnError, typename OnComplete>
		void subscribe(OnNext onNext, OnError onError, OnComplete onComplete);

	protected:
		using SubscribeMethod = std::function<void(rscpp::Subscriber<T> &)>;

		Observable(const SubscribeMethod &subscribeMethod);

		SubscribeMethod m_subscribeMethod;
	};
} // namespace recpp

#include <recpp/Observable.inl>
