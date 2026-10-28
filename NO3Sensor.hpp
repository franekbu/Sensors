#pragma once
#include <vector>
#include "Logger.hpp"

class NO3Sensor {
  private:
    std::vector<double> measurements;
    double *samples;
    int sampleCount;
    Logger log;

  public:
    NO3Sensor() = default;
    NO3Sensor(Logger log, std::vector<double> measurements = {}, double *samples=nullptr, int sampleCount=0)
        : log(log), measurements(measurements), samples(samples),
          sampleCount(sampleCount){};

    NO3Sensor(const NO3Sensor &other);
    NO3Sensor(NO3Sensor &&other) noexcept;
    ~NO3Sensor();

    void read_no3_data();
    std::vector<double> getMeasurements();
    double *getSamples();
    int getSampleCount();
};