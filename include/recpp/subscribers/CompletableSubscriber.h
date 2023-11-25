#pragma once

#include <rscpp/Subscriber.h>

namespace recpp
{
	class CompletableSubscriber
	{
	public:
		CompletableSubscriber(const rscpp::Subscriber<int> &subscriber);

		void onError(const std::exception_ptr &error);

		void onComplete();

	private:
		rscpp::Subscriber<int> m_subscriber;
		bool				   m_ended = false;
	};
} // namespace recpp
