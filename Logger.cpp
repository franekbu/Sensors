#include "Logger.hpp"

void Logger::trace(std::string prompt) { stream << prompt << std::endl; }
void Logger::debug(std::string prompt) { stream << prompt << std::endl; }
void Logger::info(std::string prompt) { stream << prompt << std::endl; }
void Logger::warn(std::string prompt) { stream << prompt << std::endl; }
void Logger::error(std::string prompt) { stream << prompt << std::endl; }
