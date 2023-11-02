export module recpp.publishers.Completable;

import rscpp;

import recpp.async.Scheduler;
import recpp.publishers.impl.CreatePublisher;
import recpp.publishers.impl.DeferPublisher;
import recpp.publishers.impl.EmptyPublisher;
import recpp.publishers.impl.ErrorPublisher;
import recpp.publishers.impl.NeverPublisher;
import recpp.processors.AndThen;
import recpp.processors.ObserveOn;
import recpp.processors.SubscribeOn;
import recpp.processors.Tap;
import recpp.subscribers.CompletableSubscriber;
import recpp.subscribers.DefaultSubscriber;

import <functional>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	class Completable : public Publisher<int>
	{
	public:
		using OnErrorMethod = function<void(const exception_ptr & /* error */)>;
		using OnCompleteMethod = function<void()>;

		static Completable complete()
		{
			return Completable(shared_ptr<Publisher<int>>(new EmptyPublisher<int>()));
		}

		static Completable create(const function<void(CompletableSubscriber<int> &)> &method)
		{
			return Completable(shared_ptr<Publisher<int>>(new CreatePublisher<int, CompletableSubscriber<int>>(method)));
		}

		static Completable defer(const function<Completable()> &function)
		{
			return Completable(shared_ptr<Publisher<int>>(new DeferPublisher<int, Completable>(function)));
		}

		static Completable error(const exception_ptr &error)
		{
			return Completable(shared_ptr<Publisher<int>>(new ErrorPublisher<int>(error)));
		}

		static Completable never()
		{
			return Completable(shared_ptr<Publisher<int>>(new NeverPublisher<int>()));
		}

		void subscribe(const OnCompleteMethod &onComplete, const OnErrorMethod &onError)
		{
			auto subscriber = DefaultSubscriber<int>(nullptr, onError, onComplete);
			Publisher<int>::subscribe(subscriber);
		}

		Completable doOnComplete(const OnCompleteMethod &method)
		{
			return Completable(shared_ptr<Publisher<int>>(new Tap<int>(*this, nullptr, nullptr, method)));
		}

		Completable doOnError(const OnErrorMethod &method)
		{
			return Completable(shared_ptr<Publisher<int>>(new Tap<int>(*this, nullptr, method, nullptr)));
		}

		Completable doOnTerminate(const OnCompleteMethod &method)
		{
			return Completable(shared_ptr<Publisher<int>>(new Tap<int>(
				*this, nullptr, [method](const exception_ptr &) { method(); }, method)));
		}

		Completable tap(const OnCompleteMethod &onCompleteMethod, const OnErrorMethod &onErrorMethod)
		{
			return Completable(shared_ptr<Publisher<int>>(new Tap<int>(*this, nullptr, onErrorMethod, onCompleteMethod)));
		}

		Completable observeOn(Scheduler &scheduler)
		{
			return Completable(shared_ptr<Publisher<int>>(new ObserveOn<int>(*this, scheduler)));
		}

		Completable subscribeOn(Scheduler &scheduler)
		{
			return Completable(shared_ptr<Publisher<int>>(new SubscribeOn<int>(*this, scheduler)));
		}

		Completable andThen(const Completable &completable)
		{
			return Completable(shared_ptr<Publisher<int>>(new AndThen<int, int>(*this, completable)));
		}

	protected:
		Completable(const shared_ptr<Publisher<int>> &dd)
			: Publisher<int>(dd)
		{
		}
	};
} // namespace recpp
