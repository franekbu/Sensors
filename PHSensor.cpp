#include "PHSensor.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

static void clearTable(double *&table, int &size) {
    delete[] table;
    table = nullptr;
    size = 0;
}

PHSensor::PHSensor(const PHSensor &other)
    : measurements(other.measurements), sampleCount(other.sampleCount),
      log(other.log) {
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
    clearTable(samples, sampleCount);
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

static double *resizeTable(int new_memory_size, int elements_count,
                           double *table_start) {

    if (new_memory_size < elements_count || new_memory_size < 0 ||
        elements_count < 0) {
        throw std::invalid_argument(
            "Nowy rozmiar pamieci nie moze byc mniejszy niz liczba elementow!");
    }

    double *new_samples = new double[new_memory_size];
    for (int i = 0; i < elements_count; i++) {
        new_samples[i] = table_start[i];
    }

    delete[] table_start;
    return new_samples;
}

void PHSensor::read_ph_data() {
    log.trace("PHSensor::read_ph_data");

    int sampleCount = 0;
    int memory_located = 10;
    double *samples = nullptr;
    try {
        std::ifstream file("ph_samples.txt");
        if (!file.is_open()) {
            throw std::ios_base::failure(
                "Nie udalo sie otworzyc pliku ph_samples.txt");
        }
        file.exceptions(std::ifstream::badbit);

        samples = new double[memory_located];
        double record;
        while (file >> record) {
            if (sampleCount == memory_located) {
                memory_located *= 2;
                samples = resizeTable(memory_located, sampleCount, samples);
            }
            samples[sampleCount] = record;
            sampleCount++;
        }
        file.close();

        if (sampleCount < memory_located) {
            samples = resizeTable(sampleCount, sampleCount, samples);
        }

    } catch (const std::ios_base::failure &e) {
        log.error(std::string("Bład I/O: ") + e.what());
        clearTable(samples, sampleCount);
    } catch (const std::invalid_argument &e) {
        log.error(std::string("Blad alokacji pamieci: ") + e.what());
        clearTable(samples, sampleCount);
    } catch (const std::bad_alloc &e) {
        log.error(std::string("Blad alokacji pamieci: ") + e.what());
        clearTable(samples, sampleCount);
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

    clearTable(this->samples, this->sampleCount);
    this->samples = samples;
    this->sampleCount = sampleCount;
    this->measurements = records;

    if (this->measurements.empty()) {
        std::cout << "Brak pomiarow do odczytu." << std::endl;
    } else {
        std::cout << "Ostatni pomiar: " << this->measurements.back()
                  << std::endl;
    }
    log.trace("PHSensor::read_ph_data");
}
