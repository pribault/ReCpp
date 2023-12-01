# Installation guide

To integrate ReCpp to your CMake based project, simply add the following lines in your CMakeLists.txt file:

```cmake
include(FetchContent)

FetchContent_Declare(
	ReCpp
	GIT_REPOSITORY	https://github.com/pribault/ReCpp.git
)

FetchContent_MakeAvailable(ReCpp)
```

The ReCpp library will then be available to be linked to your target:
```cmake
target_link_libraries(MyTarget PRIVATE ReCpp)
```
