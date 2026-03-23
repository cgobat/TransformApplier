#include "ta/engine.hpp"

#include <sstream>
#include <vector>

namespace ta {
namespace {

const std::vector<std::string> kKnownCoefficients {
    "Tub", "Tbv", "Tbr", "Tbi", "Tvr", "Tvi", "Tri",
    "Tu_ub", "Tb_bv", "Tb_br", "Tb_bi", "Tv_vr", "Tv_vi", "Tr_ri",
    "Tv_bv", "Tr_bv", "Ti_bv", "Tr_vr", "Tr_vi", "Ti_vi",
    "Eb", "Ev", "Er", "Ei", "Eu"
};

} // namespace

Engine::Engine(Config config)
    : config_(std::move(config)) {
}

ProcessResult Engine::process(const ObservationFile& input) const {
    ProcessResult result;
    result.output = input;

    std::ostringstream report;
    report << "TransformApplier portable CLI\n";
    report << "Observations: " << input.data_lines << "\n";
    report << "Comment lines: " << input.comment_lines << "\n";

    std::size_t coefficient_count = 0;
    for (const auto& key : kKnownCoefficients) {
        if (config_.coefficient(key).has_value()) {
            ++coefficient_count;
        }
    }

    report << "Recognized coefficients loaded: " << coefficient_count << "\n";

    const auto description = config_.setup_value("description");
    if (description.has_value()) {
        report << "Setup: " << *description << "\n";
    }

    report << "Status: build-modernized portable pipeline established.\n";
    report << "Status: legacy Borland/VCL GUI remains in the repository for reference.\n";
    report << "Status: cross-platform transformation logic still requires a full port from the GUI-bound legacy code.\n";

    result.report = report.str();
    result.transformed = false;
    return result;
}

} // namespace ta
