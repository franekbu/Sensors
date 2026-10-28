cmake_minimum_required(VERSION 3.10)

project(sensors)

set(CMAKE_BUILD_TYPE Debug)

add_executable(sensors main.cpp TempSensor TempSensor.cpp)

