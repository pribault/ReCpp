#pragma once

#include <recpp/processors/AndThen.h>
#include <recpp/rx/Maybe.h>
#include <recpp/rx/Observable.h>
#include <recpp/rx/Single.h>

template <typename T>
recpp::Maybe<T> recpp::Completable::andThen(const recpp::Maybe<T> &maybe)
{
	return Maybe(std::shared_ptr<rscpp::Publisher<T>>(new recpp::AndThen<int, T>(*this, maybe)));
}

template <typename T>
recpp::Observable<T> recpp::Completable::andThen(const recpp::Observable<T> &observable)
{
	return Observable(std::shared_ptr<rscpp::Publisher<T>>(new recpp::AndThen<int, T>(*this, observable)));
}

template <typename T>
recpp::Single<T> recpp::Completable::andThen(const recpp::Single<T> &maybe)
{
	return Single(std::shared_ptr<rscpp::Publisher<T>>(new recpp::AndThen<int, T>(*this, maybe)));
}
