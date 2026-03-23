#pragma once

#include <string>
#include <vector>

namespace ta {

struct ObservationFile {
    std::vector<std::string> lines;
    std::size_t comment_lines {0};
    std::size_t data_lines {0};
};

ObservationFile read_observation_file(const std::string& path);
void write_observation_file(const std::string& path, const ObservationFile& file);

} // namespace ta
