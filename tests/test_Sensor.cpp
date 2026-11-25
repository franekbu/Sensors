#include "Sensor.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <optional>

class FakeSensor : public Sensor {
  public:
    FakeSensor(Logger &logger, double min, double max)
        : Sensor(logger, min, max) {}
    std::string_view getName() const override { return "FakeSensor"; }
};

TEST(SensorTest, ConstructorStoresConfiguredRange) {
    // given
    Logger &log = Logger::instance();
    // when
    FakeSensor sensor = FakeSensor(log, 4, 9);
    // then
    EXPECT_EQ(sensor.getMinValue(), 4);
    EXPECT_EQ(sensor.getMaxValue(), 9);
}

class FakeSensorTesting : public ::testing::Test {
  protected:
    std::optional<FakeSensor> sensor;
    std::string data_path;

    void write_to_file(const std::vector<std::string> &lines) {
        data_path =
            std::filesystem::temp_directory_path() /
            ::testing::UnitTest::GetInstance()->current_test_info()->name();

        std::ofstream file(data_path, std::ios_base::out);
        ASSERT_TRUE(file.is_open());

        for (size_t i = 0; i < lines.size(); i++) {
            file << lines[i] << "\n";
        }

        file.close();
    }

    void SetUp() override {
        sensor.emplace(Logger::instance(), 4, 9);

        std::vector<std::string> file_lines = {
            "2026-11-13 23:24:51;0", "2026-11-16 23:24:51;4",
            "2026-11-19 23:24:51;6", "2026-11-21 23:24:51;9",
            "2026-11-24 23:24:51;11"};
        write_to_file(file_lines);
    }

    void TearDown() override {
        std::filesystem::remove(data_path);
        sensor.reset();
    }
};

TEST_F(FakeSensorTesting, IgnoresValuesOutsideRange) {
    sensor->read_data(data_path);
    const auto &readings = sensor->getMeasurements();
    ASSERT_EQ(readings.size(), 3);
    EXPECT_DOUBLE_EQ(readings[0].getRecord(), 4);
    EXPECT_DOUBLE_EQ(readings[1].getRecord(), 6);
    EXPECT_DOUBLE_EQ(readings[2].getRecord(), 9);
}

TEST_F(FakeSensorTesting, RepeatedReadReplacesMeasurements) {
    sensor->read_data(data_path);
    sensor->read_data(data_path);
    EXPECT_EQ(sensor->getMeasurements().size(), 3);
}

TEST_F(FakeSensorTesting, MissingFileLeavesPreviousMeasurements) {
    sensor->read_data(data_path);
    sensor->read_data(data_path + ".missing");
    EXPECT_EQ(sensor->getMeasurements().size(), 3);
}

TEST_F(FakeSensorTesting, KeepsTimestampsOfAcceptedReadings) {
    sensor->read_data(data_path);
    const auto &readings = sensor->getMeasurements();
    ASSERT_EQ(readings.size(), 3);
    EXPECT_EQ(readings[0].getTimeStamp(), "2026-11-16 23:24:51");
    EXPECT_EQ(readings[1].getTimeStamp(), "2026-11-19 23:24:51");
    EXPECT_EQ(readings[2].getTimeStamp(), "2026-11-21 23:24:51");
}

TEST_F(FakeSensorTesting, EmptyFileClearsMeasurements) {
    sensor->read_data(data_path);
    write_to_file({});
    sensor->read_data(data_path);
    const auto &readings = sensor->getMeasurements();
    EXPECT_EQ(readings.size(), 0);
}

TEST_F(FakeSensorTesting, StopsAtMalformedLine) {
    write_to_file({
        "2026-11-13 23:24:51;4", //correct line
        "2026-11-13 23:24:52;abc", //invalid line
        "2026-11-13 23:24:53;7"}); //correct line
    sensor->read_data(data_path);
    const auto &readings = sensor->getMeasurements();
    ASSERT_EQ(readings.size(), 1);
    EXPECT_DOUBLE_EQ(readings[0].getRecord(), 4);
}
