#pragma once
#include <vector>
#include "Logger.hpp"

class PHSensor {
  private:
    std::vector<double> measurements;
    double *samples;
    int sampleCount;
    Logger log;

  public:
    PHSensor() = default;
    PHSensor(Logger log, std::vector<double> measurements = {}, double *samples=nullptr, int sampleCount=0)
        : log(log), measurements(measurements), samples(samples),
          sampleCount(sampleCount){};

    PHSensor(const PHSensor &other);
    PHSensor(PHSensor &&other) noexcept;
    ~PHSensor();

    void read_ph_data();
    std::vector<double> getMeasurements();
    double *getSamples();
    int getSampleCount();
};