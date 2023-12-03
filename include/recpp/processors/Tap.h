#pragma once

#include <rscpp/Processor.h>

#include <functional>

namespace recpp::processors
{
	/**
	 * @class Tap Tap.h <recpp/processors/Tap.h>
	 * @brief {@link Processor} that will subscribe to a given {@link Publisher} and call as set of given functions on each emission.
	 *
	 * @tparam T The type of element signaled to the {@link Subscriber} and signaled from the {@link Publisher}.
	 */
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

		/**
		 * @brief Construct a new {@link Tap} instance.
		 *
		 * @param publisher The source {@link Publisher} the {@link SubscribeOn} {@link Processor} subscribes to.
		 * @param onNextMethod The function to call for each value.
		 * @param onErrorMethod The function to call on error.
		 * @param onCompleteMethod The function when the stream ends.
		 */
		explicit Tap(const rscpp::Publisher<T> &publisher, const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod,
					 const OnCompleteMethod &onCompleteMethod);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/Tap.inl>
