#pragma once

#include <rscpp/Publisher.h>

#include <functional>

namespace recpp
{
	class CompletableSubscriber;
	class Scheduler;

	class Completable : public rscpp::Publisher<int>
	{
	public:
		using OnErrorMethod = std::function<void(const std::exception_ptr & /* error */)>;
		using OnCompleteMethod = std::function<void()>;

		static Completable complete();

		static Completable create(const std::function<void(CompletableSubscriber &)> &method);

		static Completable defer(const std::function<Completable()> &function);

		static Completable error(const std::exception_ptr &error);

		static Completable never();

		void subscribe(const OnCompleteMethod &onComplete, const OnErrorMethod &onError);

		Completable doOnComplete(const OnCompleteMethod &method);

		Completable doOnError(const OnErrorMethod &method);

		Completable doOnTerminate(const OnCompleteMethod &method);

		Completable tap(const OnCompleteMethod &onCompleteMethod, const OnErrorMethod &onErrorMethod);

		Completable observeOn(recpp::Scheduler &scheduler);

		Completable subscribeOn(recpp::Scheduler &scheduler);

		Completable andThen(const Completable &completable);

	protected:
		Completable(const std::shared_ptr<rscpp::Publisher<int>> &dd);
	};
} // namespace recpp

#include <recpp/rx/inl/Completable.inl>
