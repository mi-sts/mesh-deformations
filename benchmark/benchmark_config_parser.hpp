#pragma once

#include "benchmark_config_data.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

class BenchmarkConfigParser {
public:
    static BenchmarkConfigData readConfigFile(std::fstream& config_file) {
        BenchmarkConfigData benchmark_data;
        std::string line;
        while (std::getline(config_file, line)) {
            size_t equalsPos = line.find('=');
            if (equalsPos != std::string::npos) {
                std::string key = line.substr(0, equalsPos);
                std::string value = line.substr(equalsPos + 1);
                if (key == "MAX_PARALLEL_DEFORMATIONS") {
                    benchmark_data.max_parallel_deformations = stringToInt(value);
                } else if (key == "TOTAL_DEFORMATIONS") {
                    benchmark_data.total_deformations = stringToInt(value);
                } else if (key == "MODEL_NAME") {
                    benchmark_data.model_name = value;
                } else if (key == "DEFORMATION_CLASS_NAME") {
                    benchmark_data.deformation_class_name = value;
                } else {
                    std::cerr << "Unknown config parameter.\n";
                }
            }
        }

        return benchmark_data;
    }

private:
    static int stringToInt(std::string str) {
        try {
            int value = std::stoi(str);
            return value;
        } catch (const std::invalid_argument& e) {
            std::cerr << e.what() << "\n";
            return 0;
        } catch (const std::out_of_range& e) {
            std::cerr << e.what() << "\n";
            return 0;
        }
    }
};
