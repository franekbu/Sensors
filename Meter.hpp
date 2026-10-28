#include "PHSensor.hpp"
#include "TempSensor.hpp"
#include "NO3Sensor.hpp"

class Meter {
    PHSensor ph;
    TempSensor temp;
    NO3Sensor no3;
};