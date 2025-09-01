#pragma once

#include "benchmark_config_data.hpp"
#include "deformation/deformation_controller.hpp"
#include "deformation/deformation_interface.hpp"
#include "deformation/edge_smoothing_deformation.hpp"
#include "deformation/vertex_offset_deformation.hpp"
#include "utils/filesystem.hpp"
#include "utils/mesh_loading.hpp"
#include "utils/mutithreading.hpp"

#include <atomic>
#include <chrono>
#include <memory>
#include <utility>

class BenchmarkController {
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    BenchmarkController(sptr<DeformationController> deformation_controller, const BenchmarkConfigData& config_data)
        : mesh_(std::move(loadMesh(config_data))),
          deformation_controller_(std::move(deformation_controller)),
          config_data_(config_data) {
        deformation_controller_->addMesh(mesh_);
        vertex_offset_params_ = std::make_shared<VertexOffsetParams>(generateRandomVertexOffsetMatrix(*mesh_->vertices()));
        edge_smoothing_params_ = std::make_shared<EdgeSmoothingParams>(0.5f);
        if (config_data.deformation_class_name == "VertexOffsetDeformation") {
            deformation_params_ = vertex_offset_params_;
        } else if (config_data.deformation_class_name == "EdgeSmoothingDeformation") {
            deformation_params_ = edge_smoothing_params_;
        } else {
            std::cerr << "Unknown deformation class name.\n";
        }
    }

    void start() {
        reset();
        startMeasurement();
        auto deformations_count = config_data_.max_parallel_deformations;
        for (int i = 0; i < deformations_count; ++i) {
            ++remaining_deformations_pipelines_;
            startDeformation();
        }
    }

private:
    void reset() {
        is_measurement_stopped_ = false;
        completed_deformations_.store(0);
    }

    void printBenchmarkResult(uint64_t time_millis) {
        std::cout << "Benchmark finished in " << static_cast<float>(time_millis) / 1000.0f << " seconds.\n";
        std::cout << "Deformation class name: " << config_data_.deformation_class_name << ".\n";
        std::cout << "Number of parallel deformations: " << config_data_.max_parallel_deformations << ".\n";
        std::cout << "Number of completed deformations: " << completed_deformations_
                  << ".\n";
        std::cout << "Model name: " << config_data_.model_name << ".\n";
        std::cout << "Number of vertices: " << static_cast<int64_t>(mesh_->vertices()->rows())
                  << ". Number of faces: " << static_cast<int64_t>(mesh_->faces()->rows()) << ".\n";
        std::cout << "Avg deformation time (ms): " << static_cast<float>(time_millis) / completed_deformations_ << ".\n";
    }

    void startMeasurement() {
        start_time_ = Clock::now();
    }

    void stopMeasurement() {
        auto measured_time_millis =
                std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start_time_).count();
        printBenchmarkResult(measured_time_millis);
    }

    void startDeformation() {
        auto deformation_future = deformation_controller_->applyDeformation(mesh_->family_id(), deformation_params_);
        then(deformation_future, [this](const auto& mesh) {
            ++completed_deformations_;
            if (completed_deformations_ < config_data_.total_deformations) {
                startDeformation();
            } else {
                --remaining_deformations_pipelines_;
                if (remaining_deformations_pipelines_ == 0 && !is_measurement_stopped_) {
                    std::lock_guard lock(stop_measurement_mutex_);
                    if (is_measurement_stopped_)
                        return ;

                    stopMeasurement();
                    is_measurement_stopped_ = true;
                }
            }
        });
    }

    static sptr<Mesh> loadMesh(const BenchmarkConfigData& config_data) {
        auto path = getExecutablePath() / "models" / config_data.model_name;
        return loadObjMesh(path.string());
    }

    static sptr<MatrixX3f> generateRandomVertexOffsetMatrix(const MatrixX3f& for_vertices) {
        auto offset_matrix = std::make_shared<MatrixX3f>(for_vertices);
        for (int i = 0; i < offset_matrix->rows(); ++i) {
            for (int j = 0; j < offset_matrix->row(i).size(); ++j) {
                (*offset_matrix)(i, j) = 0.05f * (static_cast<float>(rand() % 200) / 100.f - 1.f);
            }
        }

        return offset_matrix;
    }

private:
    std::atomic<int> completed_deformations_{0};
    std::atomic<int> remaining_deformations_pipelines_{0};

    TimePoint start_time_;

    sptr<Mesh> mesh_;

    sptr<DeformationController> deformation_controller_;
    BenchmarkConfigData config_data_;

    sptr<IDeformationParams> deformation_params_;
    sptr<VertexOffsetParams> vertex_offset_params_;
    sptr<EdgeSmoothingParams> edge_smoothing_params_;

    std::mutex stop_measurement_mutex_;
    bool is_measurement_stopped_{true};
};
