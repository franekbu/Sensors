#include "Meter.hpp"

void Meter::read_all(const std::string &ph_path, const std::string &temp_path,
                     const std::string &no3_path) {
    ph.read_data(ph_path);
    temp.read_data(temp_path);
    no3.read_data(no3_path);
}

const PHSensor Meter::getPHSensor() { return ph; }
const TempSensor Meter::getTempSensor() { return temp; }
const NO3Sensor Meter::getNO3Sensor() { return no3; }
