#pragma once
#include <iostream>
#include <string>

class Logger {
  private:
    std::ostream &stream;

  public:
    Logger(std::ostream &s = std::cout) : stream(s) {};
    ~Logger() = default;
    void trace(std::string prompt);
    void debug(std::string prompt);
    void info(std::string prompt);
    void warn(std::string prompt);
    void error(std::string prompt);
};