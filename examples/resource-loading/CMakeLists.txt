cmake_minimum_required(VERSION 3.8)

project(ReCpp-test
	VERSION			0.0.0
	DESCRIPTION		"ReCpp resource loading example"
	HOMEPAGE_URL	"https://github.com/pribault/ReCpp"
	LANGUAGES		CXX
)

FetchContent_Declare(
	jsoncpp
	GIT_REPOSITORY	https://github.com/open-source-parsers/jsoncpp
)
set(JSONCPP_WITH_TESTS OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(jsoncpp)

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(ReCpp-resource-loading ${SOURCES})

set_property(TARGET ReCpp-resource-loading PROPERTY CXX_STANDARD 17)

target_link_libraries(ReCpp-resource-loading ReCpp jsoncpp_static)

add_custom_target(copy_resources ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/resources) 
add_dependencies(ReCpp-resource-loading copy_resources)
