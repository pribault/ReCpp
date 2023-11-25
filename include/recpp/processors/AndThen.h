#pragma once

#include <rscpp/Processor.h>
#include <rscpp/Subscription.h>

namespace recpp
{
	template <typename T, typename R>
	class AndThen : public rscpp::Processor<T, R>
	{
		class Impl : public rscpp::Processor<T, R>
		{
		public:
			explicit Impl(rscpp::Processor<T, R> &parent, const rscpp::Publisher<T> &source, const rscpp::Publisher<R> &dest);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<R> &subscriber) override;

		private:
			rscpp::Processor<T, R> &m_parent;
			rscpp::Publisher<T>		m_source;
			rscpp::Publisher<R>		m_dest;
			rscpp::Subscriber<R>	m_subscriber;
			rscpp::Subscription		m_subscription;
			size_t					m_remaining = 0;
		};

	public:
		explicit AndThen(const rscpp::Publisher<T> &source, const rscpp::Publisher<R> &dest);
	};
} // namespace recpp

#include <recpp/processors/inl/AndThen.inl>
