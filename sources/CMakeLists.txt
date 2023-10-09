
set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/Observable.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/Observable.inl
	${CMAKE_CURRENT_SOURCE_DIR}/include/recpp/Subscription.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/Subscription.cpp
)

add_library(ReCpp STATIC ${SOURCES})

target_include_directories(ReCpp PUBLIC include)
