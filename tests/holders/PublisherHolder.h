#pragma once

// rscpp
#include <rscpp/Publisher.h>

template <typename T>
class PublisherHolder : public rscpp::Publisher<typename T::publisher_value_type>
{
public:
	template <typename... Args>
	explicit PublisherHolder(Args... args)
		: rscpp::Publisher<typename T::publisher_value_type>(std::make_shared<T>(args...))
	{
	}

	std::shared_ptr<T> dPtr() const
	{
		return static_pointer_cast<T>(rscpp::Publisher<typename T::publisher_value_type>::d_ptr);
	}
};
