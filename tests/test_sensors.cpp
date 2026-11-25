#include <gtest/gtest.h>
#include "PHSensor.hpp"
#include "TempSensor.hpp"
#include "NO3Sensor.hpp"

TEST(PHSensorTest, ReturnsName) {
    //given
    Logger &log = Logger::instance();

    //when
    PHSensor ph_sensor = PHSensor(log, 4, 9);

    //then
    EXPECT_EQ(ph_sensor.getName(), "PHSensor");
}

TEST(TempSensorTest, ReturnsName) {
    //given
    Logger &log = Logger::instance();

    //when
    TempSensor temp_sensor = TempSensor(log, 4, 9);

    //then;
    EXPECT_EQ(temp_sensor.getName(), "TempSensor");
}

TEST(NO3SensorTest, ReturnsName) {
    //given
    Logger &log = Logger::instance();

    //when
    NO3Sensor no3_sensor = NO3Sensor(log, 4, 9);

    //then
    EXPECT_EQ(no3_sensor.getName(), "NO3Sensor");
}
