#include "Logger.hpp"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <stdexcept>

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

static std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm *tm = std::localtime(&t);

    char time_str[21];
    std::strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S ", tm);
    return time_str;
}

void Logger::writeLog(LogLevel level, const std::string &log_msg) const {
    if (level >= minLevel) {
        *stream << getCurrentTimestamp() << " [" << toString(level) << "] "
                << log_msg << std::endl;
    }
}

void Logger::trace(const std::string &prompt) const {
    writeLog(LogLevel::TRACE, prompt);
}

void Logger::debug(const std::string &prompt) const {
    writeLog(LogLevel::DEBUG, prompt);
}

void Logger::info(const std::string &prompt) const {
    writeLog(LogLevel::INFO, prompt);
}

void Logger::warn(const std::string &prompt) const {
    writeLog(LogLevel::WARN, prompt);
}

void Logger::error(const std::string &prompt) const {
    writeLog(LogLevel::ERROR, prompt);
}

void Logger::configure(const std::string &output, LogLevel level) {
    if (configured) {
        throw std::logic_error("Proba ponownej konfiguracji Loggera.");
    }

    if (output == "stdout") {
        stream = &std::cout;
    } else if (output == "stderr") {
        stream = &std::cerr;
    } else {
        file.open(output, std::ios::app);
        if (!file.is_open()) {
            throw std::ios_base::failure(std::string("Blad otwarcia pliku: ") +
                                         output);
        }
        stream = &file;
    }
    minLevel = level;
    configured = true;
}
