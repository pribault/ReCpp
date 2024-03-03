cmake_minimum_required(VERSION 3.8)

project(ReCpp-test
	VERSION			0.0.0
	DESCRIPTION		"ReCpp ObserveOn and SubscribeOn processors example"
	HOMEPAGE_URL	"https://github.com/pribault/ReCpp"
	LANGUAGES		CXX
)

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(ReCpp-subscribe-and-observe-on ${SOURCES})

set_property(TARGET ReCpp-subscribe-and-observe-on PROPERTY CXX_STANDARD 17)

target_link_libraries(ReCpp-subscribe-and-observe-on ReCpp)
