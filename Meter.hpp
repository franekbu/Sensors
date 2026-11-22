#pragma once
#include "NO3Sensor.hpp"
#include "PHSensor.hpp"
#include "TempSensor.hpp"

class Meter {
  private:
    PHSensor ph;
    TempSensor temp;
    NO3Sensor no3;

  public:
    Meter(const PHSensor &ph_sensor, const TempSensor &temp_sensor,
          const NO3Sensor &no3_sensor)
        : ph(ph_sensor), temp(temp_sensor), no3(no3_sensor) {};
    void read_all(const std::string &ph_path, const std::string &temp_path,
                  const std::string &no3_path);
    const PHSensor getPHSensor();
    const TempSensor getTempSensor();
    const NO3Sensor getNO3Sensor();
};
