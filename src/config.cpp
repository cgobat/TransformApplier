#include "ta/config.hpp"

#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace ta {
namespace {

std::string trim(const std::string& value) {
    const auto begin = value.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) {
        return {};
    }
    const auto end = value.find_last_not_of(" \t\r\n");
    return value.substr(begin, end - begin + 1);
}

std::string lower(std::string value) {
    for (char& ch : value) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return value;
}

bool parse_double(const std::string& raw, double& out) {
    std::istringstream stream(raw);
    stream >> out;
    return stream && stream.eof();
}

} // namespace

Config Config::from_ini_file(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Unable to open INI file: " + path);
    }

    Config config;
    std::string section;
    std::string line;

    while (std::getline(input, line)) {
        const auto cleaned = trim(line);
        if (cleaned.empty() || cleaned[0] == ';' || cleaned[0] == '#') {
            continue;
        }
        if (cleaned.front() == '[' && cleaned.back() == ']') {
            section = lower(trim(cleaned.substr(1, cleaned.size() - 2)));
            continue;
        }

        const auto eq = cleaned.find('=');
        if (eq == std::string::npos) {
            continue;
        }

        const auto key = trim(cleaned.substr(0, eq));
        const auto value = trim(cleaned.substr(eq + 1));

        double parsed {};
        if (section == "coefficients") {
            if (parse_double(value, parsed)) {
                config.values_.coefficients[key] = parsed;
            }
        } else if (section == "error") {
            if (parse_double(value, parsed)) {
                config.values_.errors[key] = parsed;
            }
        } else if (section == "setup") {
            config.values_.setup[key] = value;
        } else if (section == "extinction") {
            config.values_.extinction[key] = value;
        }
    }

    return config;
}

std::optional<double> Config::coefficient(const std::string& key) const {
    const auto it = values_.coefficients.find(key);
    if (it == values_.coefficients.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::optional<double> Config::coefficient_error(const std::string& key) const {
    const auto it = values_.errors.find(key);
    if (it == values_.errors.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::optional<std::string> Config::setup_value(const std::string& key) const {
    const auto it = values_.setup.find(key);
    if (it == values_.setup.end()) {
        return std::nullopt;
    }
    return it->second;
}

} // namespace ta
