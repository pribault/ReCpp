#pragma once

#include <rscpp/Processor.h>

namespace recpp::processors
{
	namespace async
	{
		class Scheduler;
	}

	template <typename T>
	class SubscribeOn : public rscpp::Processor<T, T>
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
		explicit SubscribeOn(const rscpp::Publisher<T> &publisher, recpp::async::Scheduler &scheduler);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/SubscribeOn.inl>
