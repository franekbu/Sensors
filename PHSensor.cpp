#include "PHSensor.hpp"
#include <algorithm>
#include <fstream>
#include <utility>
#include <vector>
#include <iostream>

PHSensor::PHSensor(const PHSensor &other)
    : measurements(other.measurements), sampleCount(other.sampleCount) {
    samples = new double[sampleCount];
    std::copy(other.samples, other.samples + sampleCount, samples);
}

PHSensor::PHSensor(PHSensor &&other) noexcept
    : measurements(std::move(other.measurements)),
      samples(std::exchange(other.samples, nullptr)),
      sampleCount(std::exchange(other.sampleCount, 0)) {}

PHSensor::~PHSensor() {
    delete[] samples;
}

std::vector<double> PHSensor::getMeasurements() {
    return measurements;
} // pomysl nad wydajnoscią

double *PHSensor::getSamples() {
    return samples;
} // pomysl nad zabiezpieczeniem nadpisywania

int PHSensor::getSampleCount() {
    return sampleCount;
}


// popraw jakos ta metode, napisywanie tablicy do jakiejs funkcji pomocniczej
void PHSensor::read_ph_data() {

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
        std::cerr<<"Bład I/O: "<<e.what()<<std::endl;
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
        std::cerr<<"Bład I/O: "<<e.what()<<std::endl;
    }

    this->samples = samples;
    this->sampleCount = sampleCount;
    this->measurements = records;

    std::cout << "Ostatni pomiar: " << this->measurements.back() << std::endl;
}
