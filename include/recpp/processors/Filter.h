#pragma once

#include <rscpp/Processor.h>

#include <recpp/subscriptions/FilterSubscription.h>

#include <functional>
#include <vector>

namespace recpp::processors
{
	template <typename T>
	class Filter : public rscpp::Processor<T, T>
	{
		class Impl : public rscpp::Processor<T, T>
		{
		public:
			explicit Impl(rscpp::Processor<T, T> &parent, const rscpp::Publisher<T> &publisher, const std::function<bool(const T & /* value */)> &method);

			void onSubscribe(rscpp::Subscription &subscription) override;

			void onNext(const T &value) override;

			void onError(const std::exception_ptr &error) override;

			void onComplete() override;

			void subscribe(rscpp::Subscriber<T> &subscriber) override;

		private:
			rscpp::Processor<T, T>								 &m_parent;
			rscpp::Publisher<T>									  m_publisher;
			std::vector<recpp::subscriptions::FilterSubscription> m_subscriptions;
			rscpp::Subscriber<T>								  m_subscriber;
			std::function<bool(const T & /* value */)>			  m_method;
		};

	public:
		explicit Filter(const rscpp::Publisher<T> &publisher, const std::function<bool(const T & /* value */)> &method);
	};
} // namespace recpp::processors

#include <recpp/processors/inl/Filter.inl>
