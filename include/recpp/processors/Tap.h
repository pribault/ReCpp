#pragma once

#include <rscpp/Processor.h>

#include <functional>

namespace recpp::processors
{
	template <typename T>
	class Tap : public rscpp::Processor<T, T>
	{
		class Impl : public rscpp::Processor<T, T>
		{
		public:
			using OnNextMethod = std::function<void(const T & /* value */)>;
			using OnErrorMethod = std::function<void(const std::exception_ptr & /* error */)>;
			using OnCompleteMethod = std::function<void()>;

			explicit Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, const OnNextMethod &onNextMethod,
						  const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<T> &subscriber) override;

		private:
			rscpp::Processor<T, T> &m_parent;
			rscpp::Publisher<T>		m_publisher;
			rscpp::Subscriber<T>	m_subscriber;
			OnNextMethod			m_onNextMethod;
			OnErrorMethod			m_onErrorMethod;
			OnCompleteMethod		m_onCompleteMethod;
		};

	public:
		using OnNextMethod = typename Impl::OnNextMethod;
		using OnErrorMethod = typename Impl::OnErrorMethod;
		using OnCompleteMethod = typename Impl::OnCompleteMethod;

		explicit Tap(const rscpp::Publisher<T> &publisher, const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod,
					 const OnCompleteMethod &onCompleteMethod);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/Tap.inl>
