#pragma once

#include <rscpp/Processor.h>

#include <functional>

namespace recpp::processors
{
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
		explicit Map(const rscpp::Publisher<T> &publisher, const std::function<R(const T & /* value */)> &method);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/Map.inl>
