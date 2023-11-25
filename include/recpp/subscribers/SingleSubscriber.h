#pragma once

#include <rscpp/Subscriber.h>

namespace recpp
{
	template <typename T>
	class SingleSubscriber
	{
	public:
		SingleSubscriber(const rscpp::Subscriber<T> &subscriber);

		void onNext(const T &value);

		void onError(const std::exception_ptr &error);

	private:
		rscpp::Subscriber<T> m_subscriber;
		bool				 m_ended = false;
	};
} // namespace recpp

#include <recpp/subscribers/inl/SingleSubscriber.inl>
