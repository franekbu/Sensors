#pragma once
#include <string>
#include <utility>

class SensorReading {
  private:
    std::string time_stamp;
    double record;

  public:
    SensorReading(std::string time_stamp, double record)
        : time_stamp(std::move(time_stamp)), record(record) {}

    const std::string &getTimeStamp() const { return time_stamp; }
    double getRecord() const { return record; }
};
