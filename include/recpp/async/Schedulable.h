#pragma once

#include <functional>

namespace recpp::async
{
	/**
	 * @class Schedulable Schedulable.h <recpp/async/Schedulable.h>
	 * @brief Represents a potentially asynchronous action to be executed that can be canceled.
	 * <p>
	 * This class is made to be used with the {@link Scheduler} class that can be used to schedule Schedulable execution at specific points in time with the
	 * {@link Scheduler::schedule} methods.
	 */
	class Schedulable
	{
	public:
		/**
		 * @brief Construct a new Schedulable object with the given action to execute
		 *
		 * @param function the action to execute
		 */
		Schedulable(const std::function<void(void)> &function);

		/**
		 * @brief Run this Schedulable action
		 */
		void operator()();

		/**
		 * @brief Cancel this schedulable
		 */
		void cancel();

	private:
		/**
		 * @brief The action to execute
		 */
		std::function<void(void)> m_task;

		/**
		 * @brief Is this action canceled ?
		 */
		bool m_canceled = false;
	};
} // namespace recpp::async
