#pragma once

#include <rscpp/Processor.h>

#include <functional>
#include <mutex>

namespace recpp::processors
{
	/**
	 * @class Reduce Reduce.h <recpp/processors/Reduce.h>
	 * @brief {@link rscpp::Processor} that will subscribe to a given {@link rscpp::Publisher} and accumulate all its values.
	 *
	 * @param T The type of element signaled to the {@link rscpp::Subscriber}.
	 * @param R The type of element signaled by the {@link rscpp::Publisher}.
	 */
	template <typename T, typename R>
	class Reduce : public rscpp::Processor<T, T>
	{
		class Impl : public rscpp::Processor<T, T>
		{
		public:
			explicit Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, T init, R operation);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<T> &subscriber) override;

		private:
			rscpp::Processor<T, T> &m_parent;
			rscpp::Publisher<T>		m_publisher;
			rscpp::Subscriber<T>	m_subscriber;
			T						m_current;
			R						m_operation;
			std::mutex				m_mutex;
		};

	public:
		/**
		 * @brief Construct a new {@link Reduce} instance.
		 *
		 * @param publisher The source {@link rscpp::Publisher} the {@link Reduce} {@link rscpp::Processor} subscribes to.
		 * @param init The initial value.
		 * @param operation The operation to use to add two values.
		 */
		explicit Reduce(const rscpp::Publisher<T> &publisher, T init, R operation);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/Reduce.inl>
