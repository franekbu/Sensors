#pragma once
#include <vector>

class PHSensor {
  private:
    std::vector<double> measurements;
    double *samples;
    int sampleCount;

  public:
    PHSensor() = default;
    PHSensor(std::vector<double> measurements, double *samples, int sampleCount)
        : measurements(measurements), samples(samples),
          sampleCount(sampleCount) {};

    PHSensor(const PHSensor &other);
    PHSensor(PHSensor &&other) noexcept;
    ~PHSensor();

    void read_ph_data();
    std::vector<double> getMeasurements();
    double *getSamples();
    int getSampleCount();
};