#include "ta/config.hpp"
#include "ta/engine.hpp"
#include "ta/obs_file.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

namespace {

void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "test failure: " << message << '\n';
        std::exit(1);
    }
}

} // namespace

int main() {
    const std::string ini_path = "ta_test.ini";
    const std::string obs_path = "ta_test.obs";

    {
        std::ofstream ini(ini_path);
        ini << "[Setup]\n";
        ini << "description = Demo rig\n";
        ini << "[Coefficients]\n";
        ini << "Tb_bv = 0.1234\n";
        ini << "Tv_bv = 0.5678\n";
        ini << "[Error]\n";
        ini << "Tb_bv = 0.0010\n";
    }

    {
        std::ofstream obs(obs_path);
        obs << "#TYPE=Extended\n";
        obs << "STAR1,2450000.1,12.34\n";
    }

    const auto config = ta::Config::from_ini_file(ini_path);
    expect(config.setup_value("description").value_or("") == "Demo rig", "setup description parsed");
    expect(config.coefficient("Tb_bv").value_or(0.0) > 0.12, "coefficient parsed");
    expect(config.coefficient_error("Tb_bv").value_or(0.0) == 0.001, "error parsed");

    const auto obs = ta::read_observation_file(obs_path);
    expect(obs.comment_lines == 1, "comment count");
    expect(obs.data_lines == 1, "data count");

    const ta::Engine engine(config);
    const auto result = engine.process(obs);
    expect(!result.transformed, "placeholder engine reports non-transformed state");
    expect(result.report.find("Demo rig") != std::string::npos, "report includes setup");

    return 0;
}
