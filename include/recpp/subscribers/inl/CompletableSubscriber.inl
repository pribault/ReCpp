#pragma once

#include <exception>

template <typename E>
void recpp::subscribers::CompletableSubscriber::onError(E error)
{
	onError(std::make_exception_ptr(error));
}
