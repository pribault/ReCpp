export module recpp.subscribers.impl.DefaultSubscriberPrivate;

import rscpp.Subscriber;
import rscpp.Subscription;

import <exception>;
import <functional>;
import <limits>;
import <memory>;

using namespace rscpp;
using namespace std;

export namespace recpp
{
	template <typename T>
	class DefaultSubscriberPrivate : public Subscriber<T>
	{
	public:
		template <typename N, typename E, typename C>
		explicit DefaultSubscriberPrivate(N onNextMethod, E onErrorMethod, C onCompleteMethod)
			: m_onNextMethod(onNextMethod)
			, m_onErrorMethod(onErrorMethod)
			, m_onCompleteMethod(onCompleteMethod)
		{
		}

		void onSubscribe(Subscription &subscription) override
		{
			m_subscription = subscription;
			m_subscription.request(numeric_limits<size_t>::max());
		}

		void onNext(const T &value) override
		{
			if (m_onNextMethod)
				m_onNextMethod(value);
		}

		void onError(const exception_ptr &error) override
		{
			if (m_onErrorMethod)
				m_onErrorMethod(error);
		}

		void onComplete() override
		{
			if (m_onCompleteMethod)
				m_onCompleteMethod();
		}

	private:
		Subscription									  m_subscription;
		function<void(const T & /* value */)>			  m_onNextMethod;
		function<void(const exception_ptr & /* error */)> m_onErrorMethod;
		function<void()>								  m_onCompleteMethod;
	};
} // namespace recpp
