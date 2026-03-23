#pragma once

#include <map>
#include <optional>
#include <string>

namespace ta {

struct CoefficientSet {
    std::map<std::string, double> coefficients;
    std::map<std::string, double> errors;
    std::map<std::string, std::string> setup;
    std::map<std::string, std::string> extinction;
};

class Config {
public:
    static Config from_ini_file(const std::string& path);

    [[nodiscard]] const CoefficientSet& values() const noexcept { return values_; }
    [[nodiscard]] std::optional<double> coefficient(const std::string& key) const;
    [[nodiscard]] std::optional<double> coefficient_error(const std::string& key) const;
    [[nodiscard]] std::optional<std::string> setup_value(const std::string& key) const;

private:
    CoefficientSet values_;
};

} // namespace ta
