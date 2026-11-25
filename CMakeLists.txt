cmake_minimum_required(VERSION 3.14)

project(sensors LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(sensors_lib
    Logger.cpp
    Properties.cpp
    Sensor.cpp
    Meter.cpp
)
target_include_directories(sensors_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(sensors
    main.cpp
)
target_link_libraries(sensors
    PRIVATE sensors_lib
)

enable_testing()

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(unit_tests
    tests/test_sensors.cpp
    tests/test_Sensor.cpp
)
target_link_libraries(unit_tests
    PRIVATE sensors_lib GTest::gtest_main
)

gtest_discover_tests(unit_tests)