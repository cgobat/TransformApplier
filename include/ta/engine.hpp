#pragma once

#include "ta/config.hpp"
#include "ta/obs_file.hpp"

#include <string>

namespace ta {

struct ProcessResult {
    ObservationFile output;
    std::string report;
    bool transformed {false};
};

class Engine {
public:
    explicit Engine(Config config);
    [[nodiscard]] ProcessResult process(const ObservationFile& input) const;

private:
    Config config_;
};

} // namespace ta
