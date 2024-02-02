#include <recpp/rx/Completable.h>

#include <recpp/async/Scheduler.h>
#include <recpp/processors/AndThen.h>
#include <recpp/processors/ObserveOn.h>
#include <recpp/processors/SubscribeOn.h>
#include <recpp/processors/Tap.h>
#include <recpp/publishers/CreatePublisher.h>
#include <recpp/publishers/DeferPublisher.h>
#include <recpp/publishers/EmptyPublisher.h>
#include <recpp/publishers/ErrorPublisher.h>
#include <recpp/publishers/NeverPublisher.h>
#include <recpp/subscribers/CompletableSubscriber.h>
#include <recpp/subscribers/DefaultSubscriber.h>

using namespace recpp::async;
using namespace recpp::processors;
using namespace recpp::publishers;
using namespace recpp::rx;
using namespace recpp::subscribers;
using namespace rscpp;
using namespace std;

Completable Completable::complete()
{
	return Completable(make_shared<recpp::publishers::EmptyPublisher<int>>());
}

Completable Completable::create(const function<void(CompletableSubscriber &)> &method)
{
	return Completable(make_shared<recpp::publishers::CreatePublisher<int, CompletableSubscriber>>(method));
}

Completable Completable::defer(const function<Completable()> &function)
{
	return Completable(make_shared<recpp::publishers::DeferPublisher<int, Completable>>(function));
}

Completable Completable::error(const exception_ptr &error)
{
	return Completable(make_shared<recpp::publishers::ErrorPublisher<int>>(error));
}

Completable Completable::never()
{
	return Completable(make_shared<recpp::publishers::NeverPublisher<int>>());
}

void Completable::subscribe(const Completable::OnCompleteMethod &onComplete, const Completable::OnErrorMethod &onError)
{
	auto subscriber = DefaultSubscriber<int>(nullptr, onError, onComplete);
	rscpp::Publisher<int>::subscribe(subscriber);
}

Completable Completable::doOnComplete(const Completable::OnCompleteMethod &method)
{
	return Completable(make_shared<Tap<int>>(*this, nullptr, nullptr, method));
}

Completable Completable::doOnError(const Completable::OnErrorMethod &method)
{
	return Completable(make_shared<Tap<int>>(*this, nullptr, method, nullptr));
}

Completable Completable::doOnTerminate(const Completable::OnCompleteMethod &method)
{
	return Completable(make_shared<Tap<int>>(
		*this, nullptr, [method](const exception_ptr &) { method(); }, method));
}

Completable Completable::tap(const Completable::OnCompleteMethod &onCompleteMethod, const Completable::OnErrorMethod &onErrorMethod)
{
	return Completable(make_shared<Tap<int>>(*this, nullptr, onErrorMethod, onCompleteMethod));
}

Completable Completable::observeOn(Scheduler &scheduler)
{
	return Completable(make_shared<ObserveOn<int>>(*this, scheduler));
}

Completable Completable::subscribeOn(Scheduler &scheduler)
{
	return Completable(make_shared<SubscribeOn<int>>(*this, scheduler));
}

Completable Completable::andThen(const Completable &completable)
{
	return Completable(make_shared<AndThen<int, int>>(*this, completable));
}

Completable::Completable(const shared_ptr<Publisher<int>> &dd)
	: rscpp::Publisher<int>(dd)
{
}
