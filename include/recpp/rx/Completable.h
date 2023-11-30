#pragma once

#include <recpp/subscribers/CompletableSubscriber.h>
#include <rscpp/Publisher.h>

#include <functional>

namespace recpp::async
{
	class Scheduler;
}

namespace recpp::subscriber
{
	class CompletableSubscriber;
}

namespace recpp::rx
{
	template <typename T>
	class Maybe;
	template <typename T>
	class Observable;
	template <typename T>
	class Single;

	class Completable : public rscpp::Publisher<int>
	{
		template <typename T>
		friend class Maybe;
		template <typename T>
		friend class Observable;
		template <typename T>
		friend class Single;

	public:
		using OnErrorMethod = std::function<void(const std::exception_ptr & /* error */)>;
		using OnCompleteMethod = std::function<void()>;

		static Completable complete();

		static Completable create(const std::function<void(recpp::subscribers::CompletableSubscriber &)> &method);

		static Completable defer(const std::function<Completable()> &function);

		static Completable error(const std::exception_ptr &error);

		static Completable never();

		void subscribe(const OnCompleteMethod &onComplete = nullptr, const OnErrorMethod &onError = nullptr);

		Completable doOnComplete(const OnCompleteMethod &method);

		Completable doOnError(const OnErrorMethod &method);

		Completable doOnTerminate(const OnCompleteMethod &method);

		Completable tap(const OnCompleteMethod &onCompleteMethod, const OnErrorMethod &onErrorMethod);

		Completable observeOn(async::Scheduler &scheduler);

		Completable subscribeOn(async::Scheduler &scheduler);

		Completable andThen(const Completable &completable);

		template <typename T>
		Maybe<T> andThen(const Maybe<T> &maybe);

		template <typename T>
		Observable<T> andThen(const Observable<T> &observable);

		template <typename T>
		Single<T> andThen(const Single<T> &maybe);

	protected:
		Completable(const std::shared_ptr<rscpp::Publisher<int>> &dd);
	};
} // namespace recpp::rx

#include <recpp/rx/inl/Completable.inl>
