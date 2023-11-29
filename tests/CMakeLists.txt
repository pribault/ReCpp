cmake_minimum_required(VERSION 3.8)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY	https://github.com/google/googletest.git
	GIT_TAG			v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

project(ReCpp-test
	VERSION			0.0.0
	DESCRIPTION		"ReCpp test"
	HOMEPAGE_URL	"https://github.com/pribault/ReCpp"
	LANGUAGES		CXX
)

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/async/ThreadPool.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/rx/Completable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/rx/Maybe.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/rx/Observable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/rx/Single.cpp
)

add_executable(ReCpp-test ${SOURCES})

add_test(NAME ReCpp-test COMMAND ReCpp-test)

set_property(TARGET ReCpp-test PROPERTY CXX_STANDARD 17)

target_link_libraries(ReCpp-test ReCpp RsCpp gtest)
