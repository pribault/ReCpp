
add_executable(ReCpp-test)

set_property(TARGET ReCpp-test PROPERTY CXX_STANDARD 20)

target_sources(ReCpp-test
	PUBLIC
		FILE_SET CXX_MODULES FILES
			${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(ReCpp-test ReCpp)
