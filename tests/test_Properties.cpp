#include "Properties.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <stdexcept>

class PropertiesTesting : public ::testing::Test {
  protected:
    Properties props;

    std::string config_path;

    void write_to_file(const std::vector<std::string> lines) {

        std::ofstream file(config_path, std::ios_base::out);
        ASSERT_TRUE(file.is_open());

        for (size_t i = 0; i < lines.size(); i++) {
            file << lines[i] << "\n";
        }
        file.close();
    }

    void SetUp() override {

        config_path =
            std::filesystem::temp_directory_path() /
            (::testing::UnitTest::GetInstance()->current_test_info()->name() +
             std::string(".conf"));
    }
    void TearDown() override { std::filesystem::remove(config_path); }
};

TEST_F(PropertiesTesting, GetReturnsValueAndDefault) {
    write_to_file({"name = PH"});
    props.load(config_path);

    EXPECT_EQ(props.get("name"), "PH");
    EXPECT_EQ(props.get("missing"), "");
    EXPECT_EQ(props.get("missing", "def"), "def");
    EXPECT_EQ(props.has("name"), true);
    EXPECT_EQ(props.has("missing"), false);
}

TEST_F(PropertiesTesting, LoadIgnoresCommentsAndWhitespace) {
    write_to_file({"", "# comment", "  key = value   "});
    props.load(config_path);

    EXPECT_EQ(props.get("key"), "value");
    EXPECT_EQ(props.has("# comment"), false);
}

TEST_F(PropertiesTesting, LoadSplitsRangeKeyIntoMinAndMax) {
    write_to_file({"ph.range = 5-9"});
    props.load(config_path);

    EXPECT_EQ(props.get("ph.min"), "5");
    EXPECT_EQ(props.get("ph.max"), "9");
    EXPECT_EQ(props.has("ph.range"), false);
}

TEST_F(PropertiesTesting, LoadClearsPreviouslyLoadedData) {
    write_to_file({"a=1"});
    props.load(config_path);
    write_to_file({"b=5"});
    props.load(config_path);

    EXPECT_EQ(props.has("a"), false);
    EXPECT_EQ(props.get("b"), "5");
}

TEST_F(PropertiesTesting, LoadThrowsWhenFileIsMissing) {
    std::filesystem::remove(config_path);
    EXPECT_THROW(props.load(config_path), std::ios_base::failure);
}

TEST_F(PropertiesTesting, LoadThrowsWhenLineHasNoEquals) {
    write_to_file({"broken line"});
    EXPECT_THROW(props.load(config_path), std::runtime_error);
}

TEST_F(PropertiesTesting, LoadThrowsWhenKeyIsEmpty) {
    write_to_file({" = value"});
    EXPECT_THROW(props.load(config_path), std::runtime_error);
}

TEST_F(PropertiesTesting, LoadThrowsWhenValueIsEmpty) {
    write_to_file({"key = "});
    EXPECT_THROW(props.load(config_path), std::runtime_error);
}
