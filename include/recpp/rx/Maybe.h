#pragma once

#include <rscpp/Publisher.h>

#include <functional>

namespace recpp
{
	template <typename T>
	class MaybeSubscriber;
	class Scheduler;

	template <typename T>
	class Maybe : public rscpp::Publisher<T>
	{
		template <typename R>
		friend class Maybe;

	public:
		using OnNextMethod = std::function<void(const T & /* value */)>;
		using OnErrorMethod = std::function<void(const std::exception_ptr & /* error */)>;
		using OnCompleteMethod = std::function<void()>;

		static Maybe<T> create(const std::function<void(recpp::MaybeSubscriber<T> &)> &method);

		static Maybe<T> defer(const std::function<Maybe<T>()> &function);

		static Maybe<T> empty();

		static Maybe<T> error(const std::exception_ptr &error);

		static Maybe<T> just(const T &value);

		static Maybe<T> never();

		void subscribe(const OnNextMethod &onNext, const OnErrorMethod &onError, const OnCompleteMethod &onComplete);

		template <typename R>
		Maybe<R> map(const std::function<R(const T & /* value */)> &method);

		template <typename R>
		Maybe<R> flatMap(const std::function<Maybe<R>(const T & /* value */)> &method);

		Maybe<T> doOnComplete(const OnCompleteMethod &method);

		Maybe<T> doOnError(const OnErrorMethod &method);

		Maybe<T> doOnNext(const OnNextMethod &method);

		Maybe<T> doOnTerminate(const OnCompleteMethod &method);

		Maybe<T> tap(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod);

		Maybe<T> observeOn(recpp::Scheduler &scheduler);

		Maybe<T> subscribeOn(recpp::Scheduler &scheduler);

	protected:
		Maybe(const std::shared_ptr<rscpp::Publisher<T>> &dd);
	};
} // namespace recpp

#include <recpp/rx/inl/Maybe.inl>
