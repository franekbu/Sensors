cmake_minimum_required(VERSION 3.10)

project(sensors)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(
    sensors 
    main.cpp 
    TempSensor.cpp 
    PHSensor.cpp
    Logger.cpp
    NO3Sensor.cpp
    Properties.cpp
)

