#include "Meter.hpp"
#include "Sensor.hpp"
#include "SensorReading.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <optional>

class MeterTesting : public ::testing::Test {
  protected:
    std::string ph_path;
    std::string temp_path;
    std::string no3_path;
    std::string empty_path;

    std::optional<PHSensor> ph_sensor;
    std::optional<TempSensor> temp_sensor;
    std::optional<NO3Sensor> no3_sensor;
    std::optional<Meter> meter;

    void write_to_file(const std::string &path, const std::vector<std::string> lines) {
        std::ofstream file(path, std::ios_base::out);
        ASSERT_TRUE(file.is_open());

        for (size_t i = 0; i < lines.size(); i++) {
            file << lines[i] << "\n";
        }
        file.close();
    }

    void build_meter() {
        meter.emplace(*ph_sensor, *temp_sensor, *no3_sensor);
    }

    std::string get_path(const std::string &suffix) {
        return std::filesystem::temp_directory_path() /
               (::testing::UnitTest::GetInstance()
                    ->current_test_info()
                    ->name() +
                suffix);
    }

    void SetUp() override {
        ph_path = get_path("_ph.txt");
        temp_path = get_path("_temp.txt");
        no3_path = get_path("_no3.txt");
        empty_path = get_path("_empty.txt");

        write_to_file(ph_path, {"2026-11-20 10:00:00;6"});
        write_to_file(temp_path, {"2026-11-20 11:00:00;25"});
        write_to_file(no3_path, {"2026-11-20 12:00:00;75"});
        write_to_file(empty_path, {});

        ph_sensor.emplace(Logger::instance(), 1.0, 10.0);
        temp_sensor.emplace(Logger::instance(), 20.0, 30.0);
        no3_sensor.emplace(Logger::instance(), 50.0, 100.0);
    }

    void TearDown() override {
        std::filesystem::remove(ph_path);
        std::filesystem::remove(temp_path);
        std::filesystem::remove(no3_path);
        std::filesystem::remove(empty_path);
        ph_sensor.reset();
        temp_sensor.reset();
        no3_sensor.reset();
    }
};

TEST_F(MeterTesting, ReadAllDispatchesEachFileToItsSensor) {
    build_meter();
    meter->read_all(ph_path, temp_path, no3_path);
    PHSensor ph = meter->getPHSensor();
    TempSensor temp = meter->getTempSensor();
    NO3Sensor no3 = meter->getNO3Sensor();

    ASSERT_EQ(ph.getMeasurements().size(), 1);
    EXPECT_DOUBLE_EQ(ph.getMeasurements()[0].getRecord(), 6);

    ASSERT_EQ(temp.getMeasurements().size(), 1);
    EXPECT_DOUBLE_EQ(temp.getMeasurements()[0].getRecord(), 25);

    ASSERT_EQ(no3.getMeasurements().size(), 1);
    EXPECT_DOUBLE_EQ(no3.getMeasurements()[0].getRecord(), 75);
}

TEST_F(MeterTesting, ConstructorStoresCopiesOfSensors) {
    ph_sensor->read_data(ph_path);    // sensor ma wartosc 6
    build_meter();                    // sensor zostal skopiowany do meter
    ph_sensor->read_data(empty_path); // pusty plik czysci wartosci sensora
    ASSERT_EQ(ph_sensor->getMeasurements().size(), 0);

    PHSensor ph = meter->getPHSensor();
    ASSERT_EQ(ph.getMeasurements().size(), 1);
    EXPECT_DOUBLE_EQ(ph.getMeasurements()[0].getRecord(), 6);
}