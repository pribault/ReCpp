cmake_minimum_required(VERSION 3.8)

project(ReCpp-test
	VERSION			0.0.0
	DESCRIPTION		"ReCpp test"
	HOMEPAGE_URL	"https://github.com/pribault/ReCpp"
	LANGUAGES		CXX
)

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(ReCpp-test ${SOURCES})

set_property(TARGET ReCpp-test PROPERTY CXX_STANDARD 17)

target_link_libraries(ReCpp-test ReCpp RsCpp)
