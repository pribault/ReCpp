#include <recpp/async/EventLoop.h>
#include <recpp/async/WorkerThread.h>
#include <recpp/rx/Completable.h>
#include <recpp/rx/Observable.h>

#include <array>
#include <cstdlib>
#include <iostream>

using namespace recpp;
using namespace recpp::async;
using namespace recpp::rx;

namespace
{
	constexpr std::array<int, 3> values({1, 2, 3});
}

struct Context
{
	WorkerThread workerThread;
	EventLoop	 mainThreadLoop;
};

#define LOG(message) std::cout << "[thread " << std::this_thread::get_id() << "] " << message << std::endl

Completable rxSubscribeOnTest(Context &context)
{
	return Completable::defer(
			   [&context]() -> Completable
			   {
				   LOG("starting subscribeOn test");
				   return Observable<int>::range(values)
					   .doOnNext(
						   [](const auto value)
						   {
							   LOG("emitting " << value);
						   })
					   .subscribeOn(context.workerThread)
					   .doOnNext(
						   [](const auto value)
						   {
							   LOG("forwarding " << value);
						   })
					   .ignoreElements();
			   })
		.doOnComplete(
			[]()
			{
				LOG("subscribeOn test ended");
			});
}

Completable rxObserveOnTest(Context &context)
{
	return Completable::defer(
			   [&context]() -> Completable
			   {
				   LOG("starting observeOn test");
				   return Observable<int>::range(values)
					   .doOnNext(
						   [](const auto value)
						   {
							   LOG("emitting " << value);
						   })
					   .observeOn(context.workerThread)
					   .doOnNext(
						   [](const auto value)
						   {
							   LOG("forwarding " << value);
						   })
					   .ignoreElements();
			   })
		.doOnComplete(
			[]()
			{
				LOG("observeOn test ended");
			});
}

int main(int argc, char **argv)
{
	Context context;

	LOG("main thread id:   " << std::this_thread::get_id());
	LOG("worker thread id: " << context.workerThread.threadId());
	LOG("");
	rxSubscribeOnTest(context)
		.observeOn(context.mainThreadLoop) // Come back to main thread before next text
		.doOnComplete(
			[]()
			{
				LOG("");
			}) // A little bit of space between the tests
		.andThen(rxObserveOnTest(context))
		.doOnTerminate(
			[&context]()
			{
				context.mainThreadLoop.stop();
			})
		.subscribe();

	context.mainThreadLoop.run();

	return EXIT_SUCCESS;
}
