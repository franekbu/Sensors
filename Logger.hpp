#pragma once
#include <iostream>
#include <string>
#include <optional>
#include <string_view>


enum class LogLevel { TRACE, DEBUG, INFO, WARN, ERROR };

std::optional<LogLevel> parseLogLevel(std::string_view text_view);
const char *toString(LogLevel level);

class Logger {
  private:
    std::ostream &stream;
    LogLevel minLevel;

  public:
    Logger(std::ostream &s = std::cout, LogLevel level = LogLevel::TRACE)
        : stream(s), minLevel(level) {};
    ~Logger() = default;
    void trace(const std::string &prompt) const;
    void debug(const std::string &prompt) const;
    void info(const std::string &prompt) const;
    void warn(const std::string &prompt) const;
    void error(const std::string &prompt) const;
};