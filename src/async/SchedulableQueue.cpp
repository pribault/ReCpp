export module recpp.async.SchedulableQueue;

import recpp.async.Schedulable;

import <atomic>;
import <condition_variable>;
import <map>;
import <mutex>;
import <optional>;
import <ranges>;

import <iostream>;

using namespace std;

export namespace recpp
{
	class SchedulableQueue
	{
	public:
		using Clock = chrono::steady_clock;
		using Duration = Clock::duration;
		using TimePoint = Clock::time_point;

		void push(const TimePoint &timePoint, const Schedulable &schedulable)
		{
			{
				lock_guard<mutex> lock(m_dataMutex);

				if (m_stop)
					return;

				m_schedulables.insert({timePoint, schedulable});
			}

			m_cv.notify_one();
		}

		optional<Schedulable> pop(const Duration &duration)
		{
			return pop(Clock::now() + duration);
		}

		optional<Schedulable> pop(const TimePoint &until)
		{
			auto result = tryPop(Clock::now());
			if (result)
				return result;

			if (m_stop)
				return {};

			unique_lock<mutex> lock(m_cvMutex);
			m_cv.wait_until(lock, until);

			return tryPop(Clock::now());
		}

		optional<Schedulable> blockingPop()
		{
			auto result = tryPop(Clock::now());
			if (result)
				return result;

			if (m_stop)
				return {};

			unique_lock<mutex> lock(m_cvMutex);
			m_cv.wait(lock);

			return tryPop(Clock::now());
		}

		size_t size()
		{
			lock_guard<mutex> lock(m_dataMutex);

			return m_schedulables.size();
		}

		void stop()
		{
			lock_guard<mutex> lock(m_dataMutex);

			m_stop = true;
		}

		void notifyAll()
		{
			m_cv.notify_all();
		}

	private:
		optional<Schedulable> tryPop(const TimePoint &timePoint)
		{
			lock_guard<mutex> lock(m_dataMutex);

			const auto it = ranges::find_if(m_schedulables, [&timePoint](const auto &pair) { return pair.first <= timePoint; });
			if (it != m_schedulables.end())
			{
				const auto schedulable = it->second;
				m_schedulables.erase(it);
				return schedulable;
			}

			return {};
		}

		multimap<TimePoint, Schedulable> m_schedulables;
		mutex							 m_dataMutex;
		mutex							 m_cvMutex;
		condition_variable				 m_cv;
		atomic_bool						 m_stop = false;
	};
} // namespace recpp
