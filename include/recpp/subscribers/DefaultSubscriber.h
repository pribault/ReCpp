#pragma once

#include <rscpp/Subscriber.h>
#include <rscpp/Subscription.h>

#include <functional>

namespace recpp::subscribers
{
	/**
	 * @class DefaultSubscriber DefaultSubscriber.h <recpp/subscribers/DefaultSubscriber.h>
	 *
	 * @brief {@link rscpp::Subscriber} that will call a given set of functions on each signal.
	 * @param T The type of element signaled.
	 */
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
		/**
		 * @brief The type of the method to call for each emitted value.
		 */
		using OnNextMethod = std::function<void(const T & /* value */)>;

		/**
		 * @brief The type of the method to call on error.
		 */
		using OnErrorMethod = std::function<void(const std::exception_ptr & /* error */)>;

		/**
		 * @brief The type of the method to call on completion.
		 */
		using OnCompleteMethod = std::function<void()>;

		/**
		 * @brief Construct a new {@link DefaultSubscriber} instance.
		 *
		 * @param onNextMethod The function to call when a value is emitted.
		 * @param onErrorMethod The function to call when an error is emitted.
		 * @param onCompleteMethod The function to call when the {@link rscpp::Publisher} completes.
		 */
		explicit DefaultSubscriber(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod);
	};
} // namespace recpp::subscribers

#include <recpp/subscribers/inl/DefaultSubscriber.inl>
