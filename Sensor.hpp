#pragma once
#include "Logger.hpp"
#include "SensorReading.hpp"
#include <string>
#include <string_view>
#include <vector>

class Sensor {
  private:
    bool isValid(double value) const;

  protected:
    Logger log;
    double minValue;
    double maxValue;
    std::vector<SensorReading> measurements;

    Sensor(const Logger &logger, double min, double max)
        : log(logger), minValue(min), maxValue(max) {}

  public:
    void read_data(const std::string &data_path);
    const std::vector<SensorReading> &getMeasurements() const;
    double getMinValue() const;
    double getMaxValue() const;

    virtual std::string_view getName() const = 0;
    virtual ~Sensor() = default;
};
