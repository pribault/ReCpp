cmake_minimum_required(VERSION 3.28)

project(ReCpp
	VERSION			0.0.0
	DESCRIPTION		"Reactive Extensions for C++"
	HOMEPAGE_URL	"https://github.com/pribault/ReCpp"
	LANGUAGES		CXX
)

add_subdirectory(rscpp)

add_library(ReCpp)

set_property(TARGET ReCpp PROPERTY CXX_STANDARD 20)

target_sources(ReCpp
	PUBLIC
		FILE_SET CXX_MODULES FILES
			${CMAKE_CURRENT_SOURCE_DIR}/src/ReCpp.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/observables/impl/CreateObservable.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/observables/impl/DeferObservable.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/observables/impl/EmptyObservable.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/observables/impl/ErrorObservable.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/observables/impl/JustObservable.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/observables/impl/NeverObservable.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/observables/impl/RangeObservable.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/observables/Observable.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/processors/impl/FilterPrivate.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/processors/impl/FlatMapPrivate.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/processors/impl/MapPrivate.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/processors/impl/TapPrivate.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/processors/Filter.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/processors/FlatMap.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/processors/Map.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/processors/Tap.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscribers/impl/DefaultSubscriberPrivate.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscribers/DefaultSubscriber.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/impl/EmptySubscriptionPrivate.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/impl/FilterSubscriptionPrivate.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/impl/ForwardSubscriptionPrivate.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/impl/JustSubscriptionPrivate.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/impl/NeverSubscriptionPrivate.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/impl/RangeSubscriptionPrivate.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/EmptySubscription.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/FilterSubscription.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/ForwardSubscription.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/JustSubscription.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/NeverSubscription.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/RangeSubscription.cpp
)

target_link_libraries(ReCpp PRIVATE RsCpp)

add_subdirectory(tests)
