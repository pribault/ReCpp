#include <recpp/rx/Completable.h>

#include <recpp/async/Scheduler.h>
#include <recpp/publishers/CreatePublisher.h>
#include <recpp/publishers/DeferPublisher.h>
#include <recpp/publishers/EmptyPublisher.h>
#include <recpp/publishers/ErrorPublisher.h>
#include <recpp/publishers/NeverPublisher.h>
#include <recpp/processors/AndThen.h>
#include <recpp/processors/ObserveOn.h>
#include <recpp/processors/SubscribeOn.h>
#include <recpp/processors/Tap.h>
#include <recpp/subscribers/CompletableSubscriber.h>
#include <recpp/subscribers/DefaultSubscriber.h>

using namespace recpp;
using namespace rscpp;
using namespace std;

Completable Completable::complete()
{
	return Completable(shared_ptr<Publisher<int>>(new EmptyPublisher<int>()));
}

Completable Completable::create(const function<void(CompletableSubscriber &)> &method)
{
	return Completable(shared_ptr<Publisher<int>>(new CreatePublisher<int, CompletableSubscriber>(method)));
}

Completable Completable::defer(const function<Completable()> &function)
{
	return Completable(shared_ptr<Publisher<int>>(new DeferPublisher<int, Completable>(function)));
}

 Completable Completable::error(const exception_ptr &error)
{
	return Completable(shared_ptr<Publisher<int>>(new ErrorPublisher<int>(error)));
}

 Completable Completable::never()
{
	return Completable(shared_ptr<Publisher<int>>(new NeverPublisher<int>()));
}

void Completable::subscribe(const Completable::OnCompleteMethod &onComplete, const Completable::OnErrorMethod &onError)
{
	auto subscriber = DefaultSubscriber<int>(nullptr, onError, onComplete);
	Publisher<int>::subscribe(subscriber);
}

Completable Completable::doOnComplete(const Completable::OnCompleteMethod &method)
{
	return Completable(shared_ptr<Publisher<int>>(new Tap<int>(*this, nullptr, nullptr, method)));
}

Completable Completable::doOnError(const Completable::OnErrorMethod &method)
{
	return Completable(shared_ptr<Publisher<int>>(new Tap<int>(*this, nullptr, method, nullptr)));
}

Completable Completable::doOnTerminate(const Completable::OnCompleteMethod &method)
{
	return Completable(shared_ptr<Publisher<int>>(new Tap<int>(
		*this, nullptr, [method](const exception_ptr &) { method(); }, method)));
}

Completable Completable::tap(const Completable::OnCompleteMethod &onCompleteMethod, const Completable::OnErrorMethod &onErrorMethod)
{
	return Completable(shared_ptr<Publisher<int>>(new Tap<int>(*this, nullptr, onErrorMethod, onCompleteMethod)));
}

Completable Completable::observeOn(Scheduler &scheduler)
{
	return Completable(shared_ptr<Publisher<int>>(new ObserveOn<int>(*this, scheduler)));
}

Completable Completable::subscribeOn(Scheduler &scheduler)
{
	return Completable(shared_ptr<Publisher<int>>(new SubscribeOn<int>(*this, scheduler)));
}

Completable Completable::andThen(const Completable &completable)
{
	return Completable(shared_ptr<Publisher<int>>(new AndThen<int, int>(*this, completable)));
}

Completable::Completable(const shared_ptr<Publisher<int>> &dd)
	: Publisher<int>(dd)
{
}
