#pragma once

#include <rscpp/Subscription.h>

#include <functional>

namespace recpp
{
	class Subscription : public rscpp::Subscription
	{
	public:
		using RequestMethod = std::function<void(size_t)>;
		using CancelMethod = std::function<void()>;

		Subscription(const RequestMethod &requestMethod, const CancelMethod &cancelMethod);

		void request(size_t count) noexcept override;
		void cancel() noexcept override;

	protected:
		RequestMethod m_requestMethod;
		CancelMethod  m_cancelMethod;
	};
} // namespace recpp
