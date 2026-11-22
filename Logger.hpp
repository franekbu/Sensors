#pragma once
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

enum class LogLevel { TRACE, DEBUG, INFO, WARN, ERROR };

std::optional<LogLevel> parseLogLevel(std::string_view text_view);
const char *toString(LogLevel level);

class Logger {
  private:
    Logger() {};
    /*
     * Domyslnie ustawiamy stream a file puste,
     * jesli podaja file otwieramy go odrazu w atrybucie zeby lifetime pliku byl
     * zalezny od zycia singletona klasy
     * ale zeby dalej uzywac tylko stream, przypisujemy adres file do wskaznika
     * stream
     */
    LogLevel minLevel = LogLevel::TRACE;
    std::ostream *stream = &std::cerr;
    std::ofstream file;
    bool configured = false;
    void writeLog(LogLevel level, const std::string &log_msg) const;

  public:
    static Logger &instance() {
        static Logger inst;
        return inst;
    }
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    void trace(const std::string &prompt) const;
    void debug(const std::string &prompt) const;
    void info(const std::string &prompt) const;
    void warn(const std::string &prompt) const;
    void error(const std::string &prompt) const;
    void configure(const std::string &output, LogLevel level);
};