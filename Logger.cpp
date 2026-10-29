#include "Logger.hpp"

void Logger::trace(const std::string &prompt) const {
    if (minLevel <= LogLevel::TRACE) {
        stream << prompt << std::endl;
    }
}

void Logger::debug(const std::string &prompt) const {
    if (minLevel <= LogLevel::DEBUG) {
        stream << prompt << std::endl;
    }
}

void Logger::info(const std::string &prompt) const {
    if (minLevel <= LogLevel::INFO) {
        stream << prompt << std::endl;
    }
}

void Logger::warn(const std::string &prompt) const {
    if (minLevel <= LogLevel::WARN) {
        stream << prompt << std::endl;
    }
}

void Logger::error(const std::string &prompt) const {
    if (minLevel <= LogLevel::ERROR) {
        stream << prompt << std::endl;
    }
}
