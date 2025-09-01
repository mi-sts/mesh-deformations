#include "benchmark_config_parser.hpp"
#include "utils/filesystem.hpp"
#include "benchmark_controller.hpp"

#include <fstream>
#include <future>
#include <iostream>
#include <string>
#include <memory>

int main() {
    auto config_file = std::fstream(getExecutablePath() / "config.txt", std::ios::in);
    auto config_data = BenchmarkConfigParser::readConfigFile(config_file);\

    auto deformation_controller = std::make_shared<DeformationController>();
    auto benchmark_controller = std::make_shared<BenchmarkController>(deformation_controller, config_data);
    benchmark_controller->start();

    return 0;
}
