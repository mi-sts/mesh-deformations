#pragma once

#include <string>

struct BenchmarkConfigData {
    int max_parallel_deformations{0};
    int total_deformations{0};
    std::string model_name;
    std::string deformation_class_name;
};
