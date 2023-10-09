#pragma once

#include <rscpp/Subscriber.h>

#include <functional>

namespace recpp
{
	template <typename T>
	class Subscriber : public rscpp::Subscriber<T>
	{
	public:
		using OnNextMethod = std::function<void(const T &)>;
		using OnErrorMethod = std::function<void(const std::exception_ptr &)>;
		using OnCompleteMethod = std::function<void()>;

		Subscriber(const OnNextMethod &onNextMethod, const OnErrorMethod &onErrorMethod, const OnCompleteMethod &onCompleteMethod);

		void onSubscribe(rscpp::Subscription &subscription) noexcept override;

		void onNext(const T &value) noexcept override;
		void onError(const std::exception_ptr &exception) noexcept override;
		void onComplete() noexcept override;

	protected:
		OnNextMethod	 m_onNextMethod;
		OnErrorMethod	 m_onErrorMethod;
		OnCompleteMethod m_onCompleteMethod;
	};
} // namespace recpp

#include <recpp/Subscriber.inl>
