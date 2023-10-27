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
			${CMAKE_CURRENT_SOURCE_DIR}/src/observables/Observable.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/processors/Map.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscribers/DefaultSubscriber.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscribers/Subscriber.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/EmptySubscription.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/RangeSubscription.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/subscriptions/Subscription.cpp
)

target_link_libraries(ReCpp PRIVATE RsCpp)

add_subdirectory(tests)
