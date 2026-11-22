#pragma once
#include "Logger.hpp"
#include "Sensor.hpp"
#include <string_view>

class NO3Sensor : public Sensor {
  public:
    NO3Sensor(Logger &logger, double min, double max)
        : Sensor(logger, min, max) {}
    std::string_view getName() const override { return "NO3Sensor"; }
};
