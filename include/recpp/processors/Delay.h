#pragma once

#include <rscpp/Processor.h>

#include <chrono>

namespace recpp::processors
{
	namespace async
	{
		class Scheduler;
	}

	/**
	 * @class Delay Delay.h <recpp/processors/Delay.h>
	 * @brief {@link rscpp::Processor} that will subscribe to a given {@link rscpp::Publisher} and delay its emissions.
	 *
	 * @param T The type of element signaled to the {@link rscpp::Subscriber} by the {@link rscpp::Publisher}.
	 * @param Rep The duration arithmetic type representing the number of ticks.
	 * @param Period The duration period type representing the tick period (i.e. the number of second's fractions per tick)
	 */
	template <typename T, typename Rep, typename Period>
	class Delay : public rscpp::Processor<T, T>
	{
		class Impl : public rscpp::Processor<T, T>
		{
		public:
			explicit Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, recpp::async::Scheduler &scheduler,
						  const std::chrono::duration<Rep, Period> &delay, bool delayError);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<T> &subscriber) override;

		private:
			rscpp::Processor<T, T>			  &m_parent;
			rscpp::Publisher<T>				   m_publisher;
			rscpp::Subscriber<T>			   m_subscriber;
			recpp::async::Scheduler			  &m_scheduler;
			std::chrono::duration<Rep, Period> m_delayDuration;
			bool							   m_delayError = false;
		};

	public:
		/**
		 * @brief Construct a new {@link Delay} instance.
		 *
		 * @param publisher The source {@link rscpp::Publisher} the {@link FlatMap} {@link rscpp::Processor} subscribes to.
		 * @param scheduler The {@link async::Scheduler} to forward the emissions on.
		 * @param delay The delay to apply to the {@link rscpp::Publisher} emissions.
		 * @param delayError True if error must also be delayed, false otherwise.
		 */
		explicit Delay(const rscpp::Publisher<T> &publisher, recpp::async::Scheduler &scheduler, const std::chrono::duration<Rep, Period> &delay,
					   bool delayError);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/Delay.inl>
