#pragma once

#include <recpp/processors/AndThen.h>
#include <recpp/rx/Maybe.h>
#include <recpp/rx/Observable.h>
#include <recpp/rx/Single.h>

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Completable::andThen(const recpp::rx::Maybe<T> &maybe)
{
	return Maybe(std::static_pointer_cast<rscpp::Publisher<T>>(std::make_shared<recpp::processors::AndThen<int, T>>(*this, maybe)));
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Completable::andThen(const recpp::rx::Observable<T> &observable)
{
	return Observable(std::static_pointer_cast<rscpp::Publisher<T>>(std::make_shared<recpp::processors::AndThen<int, T>>(*this, observable)));
}

template <typename T>
recpp::rx::Single<T> recpp::rx::Completable::andThen(const recpp::rx::Single<T> &maybe)
{
	return Single(std::static_pointer_cast<rscpp::Publisher<T>>(std::make_shared<recpp::processors::AndThen<int, T>>(*this, maybe)));
}
