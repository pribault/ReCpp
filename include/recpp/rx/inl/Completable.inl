#pragma once

#include <recpp/processors/AndThen.h>
#include <recpp/rx/Maybe.h>
#include <recpp/rx/Observable.h>
#include <recpp/rx/Single.h>

template <typename T>
recpp::rx::Maybe<T> recpp::rx::Completable::andThen(const recpp::rx::Maybe<T> &maybe)
{
	return Maybe(std::shared_ptr<rscpp::Publisher<T>>(new recpp::processors::AndThen<int, T>(*this, maybe)));
}

template <typename T>
recpp::rx::Observable<T> recpp::rx::Completable::andThen(const recpp::rx::Observable<T> &observable)
{
	return Observable(std::shared_ptr<rscpp::Publisher<T>>(new recpp::processors::AndThen<int, T>(*this, observable)));
}

template <typename T>
recpp::rx::Single<T> recpp::rx::Completable::andThen(const recpp::rx::Single<T> &maybe)
{
	return Single(std::shared_ptr<rscpp::Publisher<T>>(new recpp::processors::AndThen<int, T>(*this, maybe)));
}
