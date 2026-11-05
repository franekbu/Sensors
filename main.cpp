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

    std::vector<TempSensor> sensors;

    try {
        std::string sensors_file =
            properties.get("sensor.temp.file", "sensors.txt");
        std::ifstream file(sensors_file);
        if (!file.is_open()) {
            throw std::ios_base::failure(
                std::string("Nie udalo sie otworzyc pliku: ") + sensors_file);
        }
        file.exceptions(std::ifstream::badbit);

        std::string temp_place;
        double temp_temp; // temporary temperature xd

        while (std::getline(file, temp_place, ',') && (file >> temp_temp)) {
            sensors.push_back(TempSensor(temp_place, temp_temp));
            /*
            pobieranie doubla zostawia znak końca lini
            ktory zostalby wczytany przy pobieraniu nowego place
            igonre przeskakuje przez napotkane \n
            dzieki czemu nie bedzie pobierane w przyszlosci
            */
            file.ignore(1000, '\n');
        }
        file.close();

    } catch (const std::ios_base::failure &e) {
        mainLog.error(std::string("Bład I/O: ") + e.what());
    }

    std::cout << "Wczytano " << sensors.size() << " sensorow." << std::endl;
    std::cout << std::left << std::setw(5) << "Lp." << " | " << std::left
              << std::setw(25) << "Nazwa sensora" << " |" << std::left
              << std::setw(12) << "Temperatura" << " |" << std::endl;

    std::cout << "------+---------------------------+-------------+"
              << std::endl;

    for (std::size_t i = 0; i < sensors.size(); i++) {
        std::cout << std::left << std::setw(5) << i + 1 << " | " << std::left
                  << std::setw(25) << sensors[i].getPlace() << " | "
                  << std::left << std::setw(11) << std::fixed
                  << std::setprecision(1) << sensors[i].getValue() << " |"
                  << std::endl;
    }

    // defaultowy na minLevel = TRACE
    PHSensor ph_sensor = PHSensor(mainLog);
    ph_sensor.read_ph_data(
        properties.get("sensor.ph.samples", "ph_samples.txt"),
        properties.get("sensor.ph.measurements", "ph_measurements.txt"));

    // PHSensor na minLevel ERROR
    // std::ostream &s = std::cout;
    // Logger errorLog = Logger(s, LogLevel::ERROR);
    NO3Sensor error_ph_sensor = NO3Sensor(mainLog);
    error_ph_sensor.read_no3_data();

    return 0;
}
