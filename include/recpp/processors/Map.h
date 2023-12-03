#pragma once

#include <rscpp/Processor.h>

#include <functional>

namespace recpp::processors
{
	/**
	 * @class Map Map.h <recpp/processors/Map.h>
	 * @brief {@link Processor} that will subscribe to a given {@link Publisher} and apply a given function to the emitted values.
	 *
	 * @param T The type of element signaled to the {@link Subscriber}.
	 * @param R The type of element signaled by the {@link Publisher}.
	 */
	template <typename T, typename R>
	class Map : public rscpp::Processor<T, R>
	{
		class Impl : public rscpp::Processor<T, R>
		{
		public:
			explicit Impl(rscpp::Processor<T, R> &parent, const rscpp::Publisher<T> &publisher, const std::function<R(const T & /* value */)> &method);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<R> &subscriber) override;

		private:
			rscpp::Processor<T, R>				   &m_parent;
			rscpp::Publisher<T>						m_publisher;
			rscpp::Subscriber<R>					m_subscriber;
			std::function<R(const T & /* value */)> m_method;
		};

	public:
		/**
		 * @brief Construct a new {@link Map} instance.
		 *
		 * @param publisher The source {@link Publisher} the {@link FlatMap} {@link Processor} subscribes to.
		 * @param method The function to apply to the {@link Publisher} values.
		 */
		explicit Map(const rscpp::Publisher<T> &publisher, const std::function<R(const T & /* value */)> &method);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/Map.inl>
