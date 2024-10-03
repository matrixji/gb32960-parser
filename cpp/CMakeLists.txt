
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(gb32960-parser
    gb32960.h
    gb32960.cpp
)

# if build test
if(BUILD_TESTING)
    add_subdirectory(test)
endif()
