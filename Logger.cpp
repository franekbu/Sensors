#include "Logger.hpp"

void Logger::trace(std::string prompt) {
    if (static_cast<int>(minLevel) <= static_cast<int>(LogLevel::TRACE)) {
        stream << prompt << std::endl;
    }
}

void Logger::debug(std::string prompt) {
    if (static_cast<int>(minLevel) <= static_cast<int>(LogLevel::DEBUG)) {
        stream << prompt << std::endl;
    }
}

void Logger::info(std::string prompt) {
    if (static_cast<int>(minLevel) <= static_cast<int>(LogLevel::INFO)) {
        stream << prompt << std::endl;
    }
}

void Logger::warn(std::string prompt) {
    if (static_cast<int>(minLevel) <= static_cast<int>(LogLevel::WARN)) {
        stream << prompt << std::endl;
    }
}

void Logger::error(std::string prompt) {
    if (static_cast<int>(minLevel) <= static_cast<int>(LogLevel::ERROR)) {
        stream << prompt << std::endl;
    }
}
