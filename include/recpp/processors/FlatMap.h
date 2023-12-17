#pragma once

#include <rscpp/Processor.h>

#include <functional>

namespace recpp::processors
{
	/**
	 * @class FlatMap FlatMap.h <recpp/processors/FlatMap.h>
	 * @brief {@link rscpp::Processor} that will subscribe to a given {@link rscpp::Publisher} and apply a given function to the emitted values.
	 * <p>
	 * The given function must return a {@link rscpp::Publisher}, the {@link FlatMap} {@link rscpp::Processor} will then subscribe to this {@link rscpp::Publisher} and forward its
	 * values.
	 *
	 * @param T The type of element signaled to the {@link rscpp::Subscriber}.
	 * @param R The type of element signaled by the {@link rscpp::Publisher}.
	 */
	template <typename T, typename R>
	class FlatMap : public rscpp::Processor<T, R>
	{
		class Impl : public rscpp::Processor<T, R>
		{
		public:
			explicit Impl(rscpp::Processor<T, R> &parent, const rscpp::Publisher<T> &publisher,
						  const std::function<rscpp::Publisher<R>(const T & /* value */)> &method);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<R> &subscriber) override;

		private:
			rscpp::Processor<T, R>									 &m_parent;
			rscpp::Publisher<T>										  m_publisher;
			rscpp::Subscriber<R>									  m_subscriber;
			std::function<rscpp::Publisher<R>(const T & /* value */)> m_method;
			size_t													  m_runningPublishers = 0;
			bool													  m_completed = false;
		};

	public:
		/**
		 * @brief Construct a new {@link FlatMap} instance.
		 * 
		 * @param publisher The source {@link rscpp::Publisher} the {@link FlatMap} {@link rscpp::Processor} subscribes to.
		 * @param method The function to apply to the {@link rscpp::Publisher} values.
		 */
		explicit FlatMap(const rscpp::Publisher<T> &publisher, const std::function<rscpp::Publisher<R>(const T & /* value */)> &method);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/FlatMap.inl>
