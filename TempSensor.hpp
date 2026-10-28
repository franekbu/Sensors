#pragma once
#include <iostream>
#include <string>

class TempSensor {
  private:
    const std::string place;
    const double value;

  public:
    TempSensor(const std::string place, const double value)
        : place(place), value(value) {};
    ~TempSensor() = default;
    std::string getPlace();
    double getValue();
};