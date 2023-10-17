cmake_minimum_required(VERSION 3.4)

project(ReCpp
	VERSION			0.0.0
	DESCRIPTION		"Reactive Extensions for C++"
	HOMEPAGE_URL	"https://github.com/pribault/ReCpp"
	LANGUAGES		CXX
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/rscpp/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/sources/include)

add_subdirectory(tests)
