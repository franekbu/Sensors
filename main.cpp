#include "Logger.hpp"
#include "NO3Sensor.hpp"
#include "PHSensor.hpp"
#include "Properties.hpp"
#include "TempSensor.hpp"
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

    std::ofstream log_file;
    std::ostream *log_stream = &std::cerr;
    std::string output = properties.get("log.output", "stderr");

    if (output == "stdout") {
        log_stream = &std::cout;
    } else if (output == "stderr") {
        log_stream = &std::cerr;
    } else {
        log_file.open(output, std::ios::app);
        if (!log_file.is_open()) {
            std::cerr << "Nie mozna otworzyc pliku logu: " << output
                      << std::endl;
            return 1;
        }
        log_stream = &log_file;
    }

    Logger mainLog(*log_stream, log_level);
    mainLog.info(std::string("Using config: ") + config_file);
    mainLog.info(std::string("Using log level: ") + toString(log_level));
    mainLog.debug(
        std::string("Wczytane ustawienia:\n" + properties.raport_properties()));

    PHSensor ph_sensor =
        PHSensor(mainLog, std::stod(properties.get("sensor.ph.min", "4")),
                 std::stod(properties.get("sensor.ph.max", "9")));

    ph_sensor.read_data(properties.get("sensor.ph.data", "ph_data.txt"));

    TempSensor temp_sensor =
        TempSensor(mainLog, std::stod(properties.get("sensor.temp.min", "0")),
                   std::stod(properties.get("sensor.temp.max", "40")));

    temp_sensor.read_data(properties.get("sensor.temp.data", "temp_data.txt"));

    NO3Sensor no3_sensor =
        NO3Sensor(mainLog, std::stod(properties.get("sensor.no3.min", "0")),
                  std::stod(properties.get("sensor.no3.max", "300")));

    no3_sensor.read_data(properties.get("sensor.no3.data", "no3_data.txt"));

    return 0;
}
