#pragma once
#include <iostream>
#include <string>

enum class LogLevel { TRACE, DEBUG, INFO, WARN, ERROR };

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