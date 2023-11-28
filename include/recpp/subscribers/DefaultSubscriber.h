#pragma once

#include <rscpp/Subscriber.h>
#include <rscpp/Subscription.h>

#include <functional>

namespace recpp
{
	template <typename T>
	class DefaultSubscriber : public rscpp::Subscriber<T>
	{
		class Impl : public rscpp::Subscriber<T>
		{
		public:
			using OnNextMethod = std::function<void(const T & /* value */)>;
			using OnErrorMethod = std::function<void(const std::exception_ptr & /* error */)>;
			using OnCompleteMethod = std::function<void()>;

			explicit Impl(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

		private:
			rscpp::Subscription m_subscription;
			OnNextMethod		m_onNextMethod;
			OnErrorMethod		m_onErrorMethod;
			OnCompleteMethod	m_onCompleteMethod;
			size_t				m_remaining = 0;
		};

	public:
		using OnNextMethod = typename Impl::OnNextMethod;
		using OnErrorMethod = typename Impl::OnErrorMethod;
		using OnCompleteMethod = typename Impl::OnCompleteMethod;

		explicit DefaultSubscriber(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod);
	};
} // namespace recpp

#include <recpp/subscribers/inl/DefaultSubscriber.inl>
