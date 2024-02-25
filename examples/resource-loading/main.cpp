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

#define LOG(message) std::cout << "[thread " << std::this_thread::get_id() << "] " << message << std::endl

class Timer
{
public:
	using Chrono = std::chrono::high_resolution_clock;

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
		return std::chrono::duration_cast<std::chrono::seconds>(m_end.value_or(now) - m_start.value_or(now)).count();
	}
	size_t elapsedMilliSeconds() const
	{
		const auto now = Chrono::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(m_end.value_or(now) - m_start.value_or(now)).count();
	}

private:
	std::optional<Chrono::time_point> m_start;
	std::optional<Chrono::time_point> m_end;
};

struct Context
{
	ThreadPool workerPool;
	EventLoop  mainThreadLoop;
};

Completable standardResourcesLoad(const std::filesystem::path &path)
{
	return Completable::defer(
		[path]() -> Completable
		{
			std::error_code error;
			const auto		dirIterator = std::filesystem::directory_iterator(path, error);
			if (error)
				return Completable::error(std::make_exception_ptr(std::runtime_error(error.message())));

			for (const auto &entry : dirIterator)
			{
				if (!entry.is_regular_file())
					continue;

				const auto				entryPath = entry.path();
				std::ifstream			stream(entryPath, std::ios_base::in | std::ios_base::binary);
				Json::Value				root;
				Json::CharReaderBuilder builder;
				builder["collectComments"] = true;
				JSONCPP_STRING errs;
				LOG("loading " << entryPath.filename());
				if (!parseFromStream(builder, stream, &root, &errs))
				{
					std::cout << errs << std::endl;
					return Completable::error(std::make_exception_ptr(std::runtime_error(errs)));
				}
			}
			return Completable::complete();
		});
}

Completable rxRunTest(const std::filesystem::path &path, const std::function<Completable(const std::filesystem::path &)> &test)
{
	return Completable::defer(
		[path, test]()
		{
			Timer timer;

			LOG("starting test");
			timer.start();
			return test(path).doOnComplete(
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

	LOG("");
	LOG("main thread id: " << std::this_thread::get_id());
	LOG("");

	const std::filesystem::path runCmd = argv[0];
	const auto					directory = runCmd.parent_path();

	rxRunTest(directory / "resources", &standardResourcesLoad)
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
						   std::rethrow_exception(exceptionPtr);
					   }
					   catch (const std::exception &exception)
					   {
						   LOG("error: " << exception.what());
					   }
				   });

	context.mainThreadLoop.run();

	return EXIT_SUCCESS;
}
