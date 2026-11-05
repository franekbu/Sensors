#pragma once
#include "Logger.hpp"
#include <vector>
#include <string>

class PHSensor {
  private:
    Logger log;
    std::vector<double> measurements;
    double *samples = nullptr;
    int sampleCount = 0;

  public:
    PHSensor() = default;
    PHSensor(Logger log, std::vector<double> measurements = {},
             double *samples = nullptr, int sampleCount = 0)
        : log(log), measurements(measurements), samples(samples),
          sampleCount(sampleCount) {};

    PHSensor(const PHSensor &other);
    PHSensor(PHSensor &&other) noexcept;
    ~PHSensor();

    void read_ph_data(const std::string &samples_path,
                      const std::string &measurements_path);
    const std::vector<double> &getMeasurements() const;
    const double *getSamples() const;
    int getSampleCount() const;
};
