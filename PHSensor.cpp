#include "PHSensor.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

PHSensor::PHSensor(const PHSensor &other)
    : measurements(other.measurements), sampleCount(other.sampleCount),
      log(log) {
    log.trace("coping PHSensor");
    samples = new double[sampleCount];
    std::copy(other.samples, other.samples + sampleCount, samples);
    log.trace("coping PHSensor");
}

PHSensor::PHSensor(PHSensor &&other) noexcept
    : measurements(std::move(other.measurements)),
      samples(std::exchange(other.samples, nullptr)),
      sampleCount(std::exchange(other.sampleCount, 0)),
      log(std::move(other.log)) {
    log.trace("moving PHSensor");
}

PHSensor::~PHSensor() {
    log.trace("~PHSensor");
    delete[] samples;
    log.trace("~PHSensor");
}

std::vector<double> PHSensor::getMeasurements() {
    log.trace("PHSensor::getMeasurements");
    return measurements;
} // pomysl nad wydajnoscią

double *PHSensor::getSamples() {
    log.trace("PHSensor::getSamples");
    return samples;
} // pomysl nad zabiezpieczeniem nadpisywania

int PHSensor::getSampleCount() {
    log.trace("PHSensor::getSampleCount");
    return sampleCount;
}

// popraw jakos ta metode, napisywanie tablicy do jakiejs funkcji pomocniczej
void PHSensor::read_ph_data() {
    log.trace("PHSensor::read_ph_data");

    int sampleCount = 0;
    int memory_located = 10;
    double *samples = new double[memory_located];
    try {
        std::ifstream file("ph_samples.txt");
        if (!file.is_open()) {
            throw std::ios_base::failure(
                "Nie udalo sie otworzyc pliku ph_samples.txt");
        }
        file.exceptions(std::ifstream::badbit);

        double record;
        while (file >> record) {
            if (sampleCount == memory_located) {
                memory_located *= 2;
                double *new_samples = new double[memory_located];

                for (size_t i = 0; i < sampleCount; i++) {
                    new_samples[i] = samples[i];
                }

                delete[] samples;
                samples = new_samples;
            }
            samples[sampleCount] = record;
            sampleCount++;
        }
        file.close();

        if (sampleCount < memory_located) {
            double *new_samples = new double[sampleCount];
            for (size_t i = 0; i < sampleCount; i++) {
                new_samples[i] = samples[i];
            }

            delete[] samples;
            samples = new_samples;
        }

    } catch (const std::ios_base::failure &e) {
        log.error(std::string("Bład I/O: ") + e.what());
    }

    std::vector<double> records;
    try {
        std::ifstream file("ph_measurements.txt");
        if (!file.is_open()) {
            throw std::ios_base::failure(
                "Nie udalo sie otworzyc pliku ph_measurements.txt");
        }
        file.exceptions(std::ifstream::badbit);

        double record;
        while (file >> record) {
            records.push_back(record);
        }
        file.close();
    } catch (const std::ios_base::failure &e) {
        log.error(std::string("Bład I/O: ") + e.what());
    }

    this->samples = samples;
    this->sampleCount = sampleCount;
    this->measurements = records;

    std::cout << "Ostatni pomiar: " << this->measurements.back() << std::endl;
    log.trace("PHSensor::read_ph_data");
}
