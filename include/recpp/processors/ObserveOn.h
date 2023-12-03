#pragma once

#include <rscpp/Processor.h>

namespace recpp::processors
{
	namespace async
	{
		class Scheduler;
	}

	/**
	 * @class ObserveOn ObserveOn.h <recpp/processors/ObserveOn.h>
	 * @brief {@link Processor} that will subscribe to a given {@link Publisher} and forward its emissions on a given {@link Scheduler}.
	 *
	 * @tparam T The type of element signaled to the {@link Subscriber} and signaled from the {@link Publisher}.
	 */
	template <typename T>
	class ObserveOn : public rscpp::Processor<T, T>
	{
		class Impl : public rscpp::Processor<T, T>
		{
		public:
			explicit Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, recpp::async::Scheduler &scheduler);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<T> &subscriber) override;

		private:
			rscpp::Processor<T, T>	&m_parent;
			rscpp::Publisher<T>		 m_publisher;
			rscpp::Subscriber<T>	 m_subscriber;
			recpp::async::Scheduler &m_scheduler;
		};

	public:
		/**
		 * @brief Construct a new {@link ObserveOn} instance.
		 *
		 * @param publisher The source {@link Publisher} the {@link ObserveOn} {@link Processor} subscribes to.
		 * @param scheduler The {@link Scheduler} to forward the emissions on.
		 */
		explicit ObserveOn(const rscpp::Publisher<T> &publisher, recpp::async::Scheduler &scheduler);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/ObserveOn.inl>
