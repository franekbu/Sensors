#include "Logger.hpp"
#include "Meter.hpp"
#include "Properties.hpp"
#include <fstream>
#include <getopt.h>
#include <iomanip>
#include <vector>

int main(int argc, char *argv[]) {

    std::optional<LogLevel> cli_level;

    std::string config_file = "config.conf";
    bool explicit_config = false;
    struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                    {"log-level", required_argument, 0, 'l'},
                                    {"config", required_argument, 0, 'c'},
                                    {0, 0, 0, 0}};
    int opt;

    while ((opt = getopt_long(argc, argv, "hc:l:", long_options, NULL)) != -1) {
        switch (opt) {
        case 'l':
            cli_level = parseLogLevel(optarg);
            if (cli_level == std::nullopt) {
                std::cerr << "Bledny poziom logowania w flagach" << std::endl;
                return 2;
            }
            break;
        case 'c':
            config_file = optarg;
            explicit_config = true;
            break;
        case 'h':
            std::cout << "Usage: " << argv[0]
                      << " [-c config] [--help] [--config config]\n";
            return 0;
        default:
            std::cerr << "Nieznana flaga: " << opt << std::endl;
            return 2;
        }
    }

    Properties properties;
    try {
        properties.load(config_file);
    } catch (const std::ios_base::failure &e) {
        if (explicit_config) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
        std::cerr << "Warning: " << e.what() << " - uzywam wartosci domyslnych"
                  << std::endl;
    } catch (const std::runtime_error &e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return 1;
    }

    LogLevel log_level = LogLevel::TRACE;
    // Poniewaz nawet jak podano flage level to sprawdzamy czy poprawnie podane
    // w config_file
    if (properties.has("log.level")) {
        auto parsed = parseLogLevel(properties.get("log.level"));
        if (!parsed) {
            std::cerr << "Zly log.level w " << config_file << std::endl;
            return 1;
        }
        log_level = *parsed;
    }
    // ale jesli podalismy flage level to jest ona wazniejsza od config wiec
    // nadpisujemy
    if (cli_level) {
        log_level = *cli_level;
    }

    std::string output = properties.get("log.output", "stderr");

    Logger &mainLog = Logger::instance();
    try {
        mainLog.configure(output, log_level);
    } catch (const std::ios_base::failure &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const std::logic_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    mainLog.info(std::string("Using config: ") + config_file);
    mainLog.info(std::string("Using log level: ") + toString(log_level));
    mainLog.debug(
        std::string("Wczytane ustawienia:\n" + properties.raport_properties()));

    PHSensor ph_sensor =
        PHSensor(mainLog, std::stod(properties.get("sensor.ph.min", "4")),
                 std::stod(properties.get("sensor.ph.max", "9")));

    TempSensor temp_sensor =
        TempSensor(mainLog, std::stod(properties.get("sensor.temp.min", "0")),
                   std::stod(properties.get("sensor.temp.max", "40")));

    NO3Sensor no3_sensor =
        NO3Sensor(mainLog, std::stod(properties.get("sensor.no3.min", "0")),
                  std::stod(properties.get("sensor.no3.max", "300")));

    Meter meter = Meter(ph_sensor, temp_sensor, no3_sensor);

    meter.read_all(properties.get("sensor.ph.data", "ph_data.txt"),
                   properties.get("sensor.temp.data", "temp_data.txt"),
                   properties.get("sensor.no3.data", "no3_data.txt"));

    return 0;
}
