#pragma once

#include <recpp/async/Schedulable.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <map>
#include <mutex>
#include <optional>

namespace recpp
{
	class SchedulableQueue
	{
	public:
		using Clock = std::chrono::steady_clock;
		using Duration = Clock::duration;
		using TimePoint = Clock::time_point;

		void push(const TimePoint &timePoint, const Schedulable &schedulable);

		std::optional<Schedulable> pop(const Duration &duration);

		std::optional<Schedulable> pop(const TimePoint &until);

		std::optional<Schedulable> blockingPop();

		size_t size();

		void stop();

		void notifyAll();

	private:
		std::optional<Schedulable> tryPop(const TimePoint &timePoint);

		std::multimap<TimePoint, Schedulable> m_schedulables;
		std::mutex							  m_dataMutex;
		std::mutex							  m_cvMutex;
		std::condition_variable				  m_cv;
		std::atomic_bool					  m_stop = false;
	};
} // namespace recpp
