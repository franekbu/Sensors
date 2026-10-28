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
    void trace(std::string prompt);
    void debug(std::string prompt);
    void info(std::string prompt);
    void warn(std::string prompt);
    void error(std::string prompt);
};