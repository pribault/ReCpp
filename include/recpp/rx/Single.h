#pragma once

#include <recpp/subscribers/SingleSubscriber.h>
#include <rscpp/Publisher.h>

#include <functional>

namespace recpp
{
	namespace async
	{
		class Scheduler;
	}

	class Completable;
	template <typename T>
	class Maybe;
	template <typename T>
	class Observable;
	template <typename T>
	class SingleSubscriber;

	template <typename T>
	class Single : public rscpp::Publisher<T>
	{
		template <typename R>
		friend class Single;
		friend class Completable;

	public:
		using OnSuccessMethod = std::function<void(const T & /* value */)>;
		using OnErrorMethod = std::function<void(const std::exception_ptr & /* error */)>;
		using OnCompleteMethod = std::function<void()>;

		static Single<T> create(const std::function<void(SingleSubscriber<T> &)> &method);

		static Single<T> defer(const std::function<Single<T>()> &function);

		static Single<T> error(const std::exception_ptr &error);

		static Single<T> just(const T &value);

		static Single<T> never();

		void subscribe(const OnSuccessMethod &onSuccess = nullptr, const OnErrorMethod &onError = nullptr);

		template <typename R>
		Single<R> map(const std::function<R(const T & /* value */)> &method);

		template <typename R>
		Single<R> flatMap(const std::function<Single<R>(const T & /* value */)> &method);

		Completable ignoreElement();

		Single<T> doOnError(const OnErrorMethod &method);

		Single<T> doOnSuccess(const OnSuccessMethod &method);

		Single<T> doOnTerminate(const OnCompleteMethod &method);

		Single<T> tap(const OnSuccessMethod &onSuccessMethod, const OnErrorMethod &onErrorMethod);

		Single<T> observeOn(async::Scheduler &scheduler);

		Single<T> subscribeOn(async::Scheduler &scheduler);

	protected:
		Single(const std::shared_ptr<rscpp::Publisher<T>> &dd);
	};
} // namespace recpp

#include <recpp/rx/inl/Single.inl>
