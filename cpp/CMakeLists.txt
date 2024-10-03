
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(gb32960-parser
        gb32960.h
        gb32960.cpp

        types/buffer_data.cpp
        types/errors.cpp
        types/header.cpp
        types/packet.cpp
        types/time.cpp
)


target_include_directories(gb32960-parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# if build test
if (BUILD_TESTING)
    add_subdirectory(test)
endif ()
