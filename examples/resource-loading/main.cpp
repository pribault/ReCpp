#include <recpp/async/EventLoop.h>
#include <recpp/async/ThreadPool.h>
#include <recpp/rx/Completable.h>
#include <recpp/rx/Observable.h>

#include <json/json.h>

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace recpp;
using namespace recpp::async;
using namespace recpp::rx;
using namespace std;

#define LOG(message) cout << "[thread " << this_thread::get_id() << "] " << message << endl

class Timer
{
public:
	using Chrono = chrono::high_resolution_clock;

	void start()
	{
		m_start = Chrono::now();
	}
	void stop()
	{
		m_end = Chrono::now();
	}
	size_t elapsedSeconds() const
	{
		const auto now = Chrono::now();
		return chrono::duration_cast<chrono::seconds>(m_end.value_or(now) - m_start.value_or(now)).count();
	}
	size_t elapsedMilliSeconds() const
	{
		const auto now = Chrono::now();
		return chrono::duration_cast<chrono::milliseconds>(m_end.value_or(now) - m_start.value_or(now)).count();
	}

private:
	optional<Chrono::time_point> m_start;
	optional<Chrono::time_point> m_end;
};

struct Context
{
	ThreadPool workerPool;
	EventLoop  mainThreadLoop;
};

Completable standardResourcesLoad(Context &context, const filesystem::path &path)
{
	return Completable::defer(
		[path]() -> Completable
		{
			error_code error;
			const auto dirIterator = filesystem::directory_iterator(path, error);
			if (error)
				return Completable::error(make_exception_ptr(runtime_error(error.message())));

			for (const auto &entry : dirIterator)
			{
				if (!entry.is_regular_file())
					continue;

				const auto				entryPath = entry.path();
				ifstream				stream(entryPath, ios_base::in | ios_base::binary);
				Json::Value				root;
				Json::CharReaderBuilder builder;
				builder["collectComments"] = true;
				JSONCPP_STRING errs;
				if (!parseFromStream(builder, stream, &root, &errs))
				{
					cout << errs << endl;
					return Completable::error(make_exception_ptr(runtime_error(errs)));
				}
			}
			return Completable::complete();
		});
}

Completable rxResourcesLoad(Context &context, const filesystem::path &path)
{
	Observable<Completable> completableSource = Observable<Completable>::create(
		[&context, path](auto &subscriber)
		{
			error_code error;
			const auto dirIterator = filesystem::directory_iterator(path, error);
			if (error)
			{
				subscriber.onError(make_exception_ptr(runtime_error(error.message())));
				return;
			}

			for (const auto &entry : dirIterator)
			{
				if (!entry.is_regular_file())
					continue;

				const auto entryPath = entry.path();
				subscriber.onNext(Completable::defer(
									  [entryPath]()
									  {
										  ifstream				  stream(entryPath, ios_base::in | ios_base::binary);
										  Json::Value			  root;
										  Json::CharReaderBuilder builder;
										  builder["collectComments"] = true;
										  JSONCPP_STRING errs;
										  if (!parseFromStream(builder, stream, &root, &errs))
										  {
											  cout << errs << endl;
											  return Completable::error(make_exception_ptr(runtime_error(errs)));
										  }
										  return Completable::complete();
									  })
									  .subscribeOn(context.workerPool)
									  .observeOn(context.mainThreadLoop));
			}
			subscriber.onComplete();
		});
	return Completable::merge(completableSource);
}

Completable rxRunTest(Context &context, const string &testName, const filesystem::path &path,
					  const function<Completable(Context &, const filesystem::path &)> &test)
{
	return Completable::defer(
		[&context, testName, path, test]()
		{
			Timer timer;

			LOG("starting '" << testName << "' test");
			timer.start();
			return test(context, path)
				.doOnComplete(
					[timer]() mutable
					{
						timer.stop();
						LOG("test ended, took " << timer.elapsedMilliSeconds() << "ms");
					});
		});
}

int main(int argc, char **argv)
{
	Context context;

	LOG("main thread id: " << this_thread::get_id());
	LOG("");

	const filesystem::path runCmd = argv[0];
	const auto			   directory = runCmd.parent_path();

	rxRunTest(context, "standard", directory / "resources", &standardResourcesLoad)
		.doOnComplete(
			[]()
			{
				LOG("");
			})
		.andThen(rxRunTest(context, "reactive", directory / "resources", &rxResourcesLoad))
		.doOnTerminate(
			[&context]()
			{
				context.mainThreadLoop.stop();
			})
		.subscribe({},
				   [](const auto &exceptionPtr)
				   {
					   try
					   {
						   rethrow_exception(exceptionPtr);
					   }
					   catch (const exception &exception)
					   {
						   LOG("error: " << exception.what());
					   }
				   });

	context.mainThreadLoop.run();

	return EXIT_SUCCESS;
}
