#include "ta/config.hpp"
#include "ta/engine.hpp"
#include "ta/obs_file.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <string>

namespace {

void print_usage() {
    std::cout
        << "Usage: ta_cli --input <file> --output <file> --coefficients <file> [--report <file>]\n";
}

std::string read_arg(int& index, int argc, char* argv[]) {
    if (index + 1 >= argc) {
        throw std::runtime_error("Missing value for argument: " + std::string(argv[index]));
    }
    ++index;
    return argv[index];
}

} // namespace

int main(int argc, char* argv[]) {
    try {
        if (argc == 1) {
            print_usage();
            return 1;
        }

        std::string input_path;
        std::string output_path;
        std::string coefficients_path;
        std::string report_path;

        for (int index = 1; index < argc; ++index) {
            const std::string arg = argv[index];
            if (arg == "--input") {
                input_path = read_arg(index, argc, argv);
            } else if (arg == "--output") {
                output_path = read_arg(index, argc, argv);
            } else if (arg == "--coefficients") {
                coefficients_path = read_arg(index, argc, argv);
            } else if (arg == "--report") {
                report_path = read_arg(index, argc, argv);
            } else if (arg == "--help" || arg == "-h") {
                print_usage();
                return 0;
            } else {
                throw std::runtime_error("Unknown argument: " + arg);
            }
        }

        if (input_path.empty() || output_path.empty() || coefficients_path.empty()) {
            print_usage();
            return 1;
        }

        const auto config = ta::Config::from_ini_file(coefficients_path);
        const auto input = ta::read_observation_file(input_path);
        const ta::Engine engine(config);
        const auto result = engine.process(input);

        ta::write_observation_file(output_path, result.output);

        if (!report_path.empty()) {
            std::ofstream report_file(report_path, std::ios::binary);
            if (!report_file) {
                throw std::runtime_error("Unable to write report file: " + report_path);
            }
            report_file << result.report;
        } else {
            std::cout << result.report;
        }

        return result.transformed ? 0 : 2;
    } catch (const std::exception& ex) {
        std::cerr << "error: " << ex.what() << '\n';
        return 1;
    }
}
