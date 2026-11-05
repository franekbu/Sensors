#include "Logger.hpp"
#include <algorithm>
#include <cctype>

// TRACE, DEBUG, INFO, WARN, ERROR
std::optional<LogLevel> parseLogLevel(std::string_view text_view) {
    std::string text{text_view};
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
        return static_cast<char>(std::toupper(c));
    });

    if (text == "TRACE") {
        return LogLevel::TRACE;
    } else if (text == "DEBUG") {
        return LogLevel::DEBUG;
    } else if (text == "INFO") {
        return LogLevel::INFO;
    } else if (text == "WARN") {
        return LogLevel::WARN;
    } else if (text == "ERROR") {
        return LogLevel::ERROR;
    } else {
        return std::nullopt;
    }
}

const char *toString(LogLevel level) {
    switch (level) {
    case LogLevel::TRACE:
        return "TRACE";
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARN:
        return "WARN";
    case LogLevel::ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

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
