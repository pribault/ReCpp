#pragma once

#include <rscpp/Publisher.h>

#include <functional>

namespace recpp::publishers
{
	/**
	 * @class CreatePublisher CreatePublisher.h <recpp/publishers/CreatePublisher.h>
	 * @brief {@link Publisher} that will call a given method on subscription, with a newly created subscriber as parameter that will be used to emit values,
	 * errors and stream end.
	 *
	 * @param T The type of element signaled by the {@link Publisher}.
	 * @param S The type of subscriber that will be passed to the creation method
	 */
	template <typename T, typename S>
	class CreatePublisher : public rscpp::Publisher<T>
	{
	public:
		using CreateMethod = std::function<void(S & /* subscriber */)>;

		/**
		 * @brief Construct a new {@link CreatePublisher} instance.
		 *
		 * @param method The method to call on subscription.
		 */
		CreatePublisher(const CreateMethod &method);

		/**
		 * @brief Request {@link Publisher} to start streaming data.
		 * <p>
		 * This is a "factory method" and can be called multiple times, each time starting a new {@link Subscription}.
		 * <p>
		 * Each {@link Subscription} will work for only a single {@link Subscriber}.
		 * <p>
		 * A {@link Subscriber} should only subscribe once to a single {@link Publisher}.
		 * <p>
		 * If the {@link Publisher} rejects the subscription attempt or otherwise fails it will
		 * signal the error via {@link Subscriber::onError(const std::exception_ptr &)}.
		 *
		 * @param subscriber the {@link Subscriber} that will consume signals from this {@link Publisher}
		 */
		void subscribe(rscpp::Subscriber<T> &subscriber) override;

	private:
		CreateMethod m_method;
	};
} // namespace recpp::publishers

#include <recpp/publishers/inl/CreatePublisher.inl>
