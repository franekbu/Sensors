#include "Properties.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

bool Properties::has(const std::string &key) const {
    return properties.find(key) != properties.end();
}

std::string Properties::get(const std::string &key,
                            const std::string &def) const {
    auto value = properties.find(key);
    if (value != properties.end()) {
        return value->second;
    }
    return def;
}

static void trim(std::string &line) {
    std::size_t start = line.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        line.clear();
        return;
    }
    std::size_t end = line.find_last_not_of(" \t\n\r");
    line = line.substr(start, end - start + 1);
}

void Properties::load(const std::string &filename) {

    std::ifstream conf_file(filename);
    if (!conf_file.is_open()) {
        throw std::ios_base::failure(std::string("Blad otwarcia pliku: ") +
                                     filename);
    }
    conf_file.exceptions(std::ifstream::badbit);

    std::string line;
    std::size_t line_id = 0;
    properties.clear();
    while (std::getline(conf_file, line)) {
        line_id++;
        trim(line);

        if (line.empty())
            continue;

        if (line[0] == '#')
            continue;

        std::size_t equal = line.find('=');
        if (equal == std::string::npos) {
            throw std::runtime_error(std::string("Blad pliku: ") + filename +
                                     std::string(", brak = na lini nr:") +
                                     std::to_string(line_id));
        }

        std::string key = line.substr(0, equal);
        std::string value = line.substr(equal + 1);
        trim(key);
        trim(value);
        if (key.empty()) {
            throw std::runtime_error(std::string("Blad pliku: ") + filename +
                                     std::string(", pusty klucz na lini nr:") +
                                     std::to_string(line_id));
        }
        if (value.empty()) {
            throw std::runtime_error(
                std::string("Blad pliku: ") + filename +
                std::string(", pusta wartosc na lini nr:") +
                std::to_string(line_id));
        }

        properties[key] = value;
    }
}