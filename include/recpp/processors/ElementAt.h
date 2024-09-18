#pragma once

#include <rscpp/Processor.h>

#include <functional>
#include <mutex>

namespace recpp::processors
{
	/**
	 * @class ElementAt ElementAt.h <recpp/processors/ElementAt.h>
	 * @brief {@link rscpp::Processor} that will subscribe to a given {@link rscpp::Publisher}, retrieve the element at a specified index and emit it. If the
	 * element is out of range a default constructed value will be emited.
	 *
	 * @param T The type of element signaled to the {@link rscpp::Subscriber}.
	 */
	template <typename T>
	class ElementAt : public rscpp::Processor<T, T>
	{
		class Impl : public rscpp::Processor<T, T>
		{
		public:
			explicit Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, std::size_t index);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<T> &subscriber) override;

		private:
			rscpp::Processor<T, T> &m_parent;
			rscpp::Publisher<T>		m_publisher;
			rscpp::Subscriber<T>	m_subscriber;
			rscpp::Subscription		m_subscription;
			std::atomic_size_t		m_distance;
			std::optional<T>		m_result;
		};

	public:
		/**
		 * @brief Construct a new {@link ElementAt} instance.
		 *
		 * @param publisher The source {@link rscpp::Publisher} the {@link ElementAt} {@link rscpp::Processor} subscribes to.
		 * @param index The element index.
		 */
		explicit ElementAt(const rscpp::Publisher<T> &publisher, std::size_t index);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/ElementAt.inl>
