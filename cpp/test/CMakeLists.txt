
ENABLE_TESTING()

set(USE_GTEST_FROM "auto" CACHE STRING "Select GTest installation method")
set_property(CACHE USE_GTEST_FROM PROPERTY STRINGS "auto" "system" "source")

set(GTEST_DOWNLOAD_URL "https://github.com/google/googletest/archive/release-1.15.2.tar.gz"
        CACHE STRING "GTest download URL")


include(FindGTest)
include(FetchContent)

FetchContent_Declare(
        googletest
        URL ${GTEST_DOWNLOAD_URL}
        DOWNLOAD_EXTRACT_TIMESTAMP ON
)

if (USE_GTEST_FROM STREQUAL "system")
    find_package(GTest)
    if (NOT GTEST_FOUND)
        message(FATAL_ERROR "GTest not found")
    endif ()
elseif (USE_GTEST_FROM STREQUAL "auto")
    find_package(GTest)
    if (NOT GTest_FOUND)
        FetchContent_MakeAvailable(googletest)
    endif ()
else ()
    FetchContent_MakeAvailable(googletest)
endif ()

add_executable(test_gb32960_parser
        test_types.cpp
)
target_include_directories(test_gb32960_parser PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../
        ${GTEST_INCLUDE_DIRS}
)
target_link_libraries(test_gb32960_parser PRIVATE
        gb32960-parser
        GTest::GTest
        GTest::Main
)

