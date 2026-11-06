#pragma once
#include "Logger.hpp"
#include "Sensor.hpp"
#include <string_view>

class PHSensor : public Sensor {
  public:
    PHSensor(const Logger &logger, double min, double max)
        : Sensor(logger, min, max) {}
    std::string_view getName() const override { return "PHSensor"; }
};
