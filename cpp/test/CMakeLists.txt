
ENABLE_TESTING()

set(USE_GTEST_FROM "auto" CACHE STRING "Select GTest installation method")
set_property(CACHE USE_GTEST_FROM PROPERTY STRINGS "auto" "system" "source")

set(GTEST_DOWNLOAD_URL "https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz"
        CACHE STRING "GTest download URL")


include(FindGTest)
include(FetchContent)

if (USE_GTEST_FROM STREQUAL "system")
    find_package(GTest REQUIRED)
    message(STATUS "GTest from system, ${GTEST_INCLUDE_DIRS}")
elseif (USE_GTEST_FROM STREQUAL "auto")
    find_package(GTest)
endif ()

if (NOT GTest_FOUND)
    message(STATUS "GTest not found, download fom ${GTEST_DOWNLOAD_URL}")
    FetchContent_Declare(
            googletest
            URL ${GTEST_DOWNLOAD_URL}
            DOWNLOAD_EXTRACT_TIMESTAMP ON
            FIND_PACKAGE_ARGS NAMES GTest
    )
    FetchContent_MakeAvailable(googletest)
    message(STATUS "GTest from source, ${GTEST_INCLUDE_DIRS}")
endif ()

add_executable(test_gb32960_parser
        test_types.cpp
        test_hello.cpp
)
target_include_directories(test_gb32960_parser PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../
        ${GTEST_INCLUDE_DIRS}
)
target_link_libraries(test_gb32960_parser PRIVATE
        gb32960-parser
        ${GTEST_BOTH_LIBRARIES}
)

