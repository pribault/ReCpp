
set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(ReCpp-test ${SOURCES})

target_include_directories(ReCpp-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
