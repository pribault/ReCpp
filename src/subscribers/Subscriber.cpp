export module recpp.subscribers.Subscriber;

import rscpp.Subscriber;

export namespace recpp
{
	template <typename T>
	class Subscriber : public rscpp::Subscriber<T>
	{
	public:
		Subscriber(const typename rscpp::Subscriber<T>::OnSubscribeMethod &onSubscribeMethod, const typename rscpp::Subscriber<T>::OnNextMethod &onNextMethod,
				   const typename rscpp::Subscriber<T>::OnErrorMethod &onErrorMethod, const typename rscpp::Subscriber<T>::OnCompleteMethod &onCompleteMethod,
				   const rscpp::Subscriber<T>::StatePtr &state = nullptr)
			: rscpp::Subscriber<T>(onSubscribeMethod, onNextMethod, onErrorMethod, onCompleteMethod, state)
		{
		}
	};
} // namespace recpp
