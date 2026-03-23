#include "ta/obs_file.hpp"

#include <fstream>
#include <stdexcept>

namespace ta {

ObservationFile read_observation_file(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Unable to open observation file: " + path);
    }

    ObservationFile file;
    std::string line;
    while (std::getline(input, line)) {
        if (!line.empty() && line.front() == '#') {
            ++file.comment_lines;
        } else if (!line.empty()) {
            ++file.data_lines;
        }
        file.lines.push_back(line);
    }
    return file;
}

void write_observation_file(const std::string& path, const ObservationFile& file) {
    std::ofstream output(path, std::ios::binary);
    if (!output) {
        throw std::runtime_error("Unable to write observation file: " + path);
    }

    for (std::size_t index = 0; index < file.lines.size(); ++index) {
        output << file.lines[index];
        if (index + 1 < file.lines.size()) {
            output << '\n';
        }
    }
}

} // namespace ta
