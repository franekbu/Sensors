#include "Sensor.hpp"
#include <fstream>
#include <utility>

bool Sensor::isValid(double value) const {
    return value >= minValue && value <= maxValue;
}

const std::vector<SensorReading> &Sensor::getMeasurements() const {
    return measurements;
}

double Sensor::getMinValue() const { return minValue; }
double Sensor::getMaxValue() const { return maxValue; }

void Sensor::read_data(const std::string &data_path) {
    log.trace(std::string(getName()) + "::read_data (start)");
    try {
        std::vector<SensorReading> temp_meas;
        std::ifstream file(data_path);
        if (!file.is_open()) {
            throw std::ios_base::failure(
                std::string("Nie udalo sie otworzyc pliku: ") + data_path);
        }
        file.exceptions(std::ifstream::badbit);

        double temp_value;
        std::string temp_time;
        while (std::getline(file, temp_time, ';') && (file >> temp_value)) {
            if (isValid(temp_value)) {
                temp_meas.push_back(
                    SensorReading(std::move(temp_time), temp_value));
            } else {
                log.warn(temp_time + std::string(" wartosc ") +
                         std::to_string(temp_value) +
                         std::string(" poza zakresem."));
            }
            file.ignore(1000, '\n');
        }
        file.close();
        measurements = std::move(temp_meas);

    } catch (const std::ios_base::failure &e) {
        log.error(std::string("Bład I/O: ") + e.what());
    }
    log.trace(std::string(getName()) + "::read_data (end)");
}
