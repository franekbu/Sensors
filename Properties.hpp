#pragma once
#include <string>
#include <unordered_map>

class Properties {
  private:
    std::unordered_map<std::string, std::string> properties;

  public:
    void load(const std::string &filename);
    std::string get(const std::string &key, const std::string &def = "") const;
    bool has(const std::string &key) const;
    std::string raport_properties() const;
};