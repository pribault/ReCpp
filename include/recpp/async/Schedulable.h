#pragma once

#include <functional>

namespace recpp::async
{
	class Schedulable
	{
	public:
		Schedulable(const std::function<void(void)> &function);

		void operator()();

		void cancel();

	private:
		std::function<void(void)> m_task;
		bool					  m_canceled = false;
	};
} // namespace recpp::async
