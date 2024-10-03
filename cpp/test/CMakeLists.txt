
ENABLE_TESTING()

set(USE_GTEST_FROM "source" CACHE STRING "Select GTest installation method")
set_property(CACHE USE_GTEST_FROM PROPERTY STRINGS "auto" "system" "source")

set(GTEST_DOWNLOAD_URL "https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz"
        CACHE STRING "GTest download URL")


include(FetchContent)

message(STATUS "GTest installation method: ${USE_GTEST_FROM}")

if (USE_GTEST_FROM STREQUAL "system")
    find_package(GTest REQUIRED)
elseif (USE_GTEST_FROM STREQUAL "auto")
    find_package(GTest)
endif ()

set(test_dependency_libs gtest gtest_main)

# DOWNLOAD_EXTRACT_TIMESTAMP
if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif ()

if (GTest_FOUND)
    message(STATUS "GTest from system")
    set(test_dependency_libs GTest::GTest GTest::Main)
else ()
    message(STATUS "GTest not found, download fom ${GTEST_DOWNLOAD_URL}")
    FetchContent_Declare(
            googletest
            URL ${GTEST_DOWNLOAD_URL}
            EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(googletest)
    message(STATUS "GTest from source")
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
        ${test_dependency_libs}
)

