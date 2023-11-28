cmake_minimum_required(VERSION 3.8)

include(FetchContent)

FetchContent_Declare(
	RsCpp
	GIT_REPOSITORY	https://github.com/pribault/RsCpp.git
	GIT_TAG			2c16b1533453586282a06f00c2313a95a4d33ba2
)
FetchContent_MakeAvailable(RsCpp)

project(ReCpp
	VERSION			0.0.0
	DESCRIPTION		"Reactive Extensions for C++"
	HOMEPAGE_URL	"https://github.com/pribault/ReCpp"
	LANGUAGES		CXX
)

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/async/EventLoop.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/async/Schedulable.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/async/SchedulableQueue.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/async/Scheduler.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/async/ThreadPool.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/async/ThreadPoolWorker.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/async/WorkerThread.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/AndThen.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/Filter.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/FlatMap.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/IgnoreElements.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/Map.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/ObserveOn.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/SubscribeOn.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/Tap.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/inl/AndThen.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/inl/Filter.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/inl/FlatMap.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/inl/IgnoreElements.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/inl/Map.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/inl/ObserveOn.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/inl/SubscribeOn.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/processors/inl/Tap.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/publishers/CreatePublisher.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/publishers/DeferPublisher.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/publishers/EmptyPublisher.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/publishers/ErrorPublisher.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/publishers/JustPublisher.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/publishers/NeverPublisher.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/publishers/RangePublisher.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/publishers/inl/CreatePublisher.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/publishers/inl/DeferPublisher.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/publishers/inl/EmptyPublisher.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/publishers/inl/ErrorPublisher.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/publishers/inl/JustPublisher.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/publishers/inl/NeverPublisher.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/publishers/inl/RangePublisher.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/rx/Completable.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/rx/Maybe.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/rx/Observable.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/rx/Single.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/rx/inl/Completable.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/rx/inl/Maybe.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/rx/inl/Observable.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/rx/inl/Single.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscribers/CompletableSubscriber.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscribers/DefaultSubscriber.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscribers/MaybeSubscriber.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscribers/ObservableSubscriber.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscribers/SingleSubscriber.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscribers/inl/DefaultSubscriber.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscribers/inl/MaybeSubscriber.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscribers/inl/ObservableSubscriber.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscribers/inl/SingleSubscriber.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscriptions/EmptySubscription.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscriptions/FilterSubscription.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscriptions/ForwardSubscription.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscriptions/JustSubscription.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscriptions/NeverSubscription.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscriptions/RangeSubscription.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscriptions/inl/EmptySubscription.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscriptions/inl/JustSubscription.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscriptions/inl/NeverSubscription.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/subscriptions/inl/RangeSubscription.inl
	${CMAKE_CURRENT_SOURCE_DIR}/src/async/EventLoop.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/async/Schedulable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/async/SchedulableQueue.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/async/Scheduler.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/async/ThreadPool.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/async/ThreadPoolWorker.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/async/WorkerThread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/rx/Completable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/subscribers/CompletableSubscriber.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/FilterSubscription.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/ForwardSubscription.cpp
)

add_library(ReCpp ${SOURCES})

target_include_directories(ReCpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ReCpp PRIVATE RsCpp)

set_property(TARGET ReCpp PROPERTY CXX_STANDARD 17)

include(CTest)
add_subdirectory(tests)
