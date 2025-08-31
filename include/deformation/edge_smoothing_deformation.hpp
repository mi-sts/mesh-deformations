#pragma once

#include "deformation_interface.hpp"
#include "deformation_params_interface.hpp"
#include "structures/mesh.hpp"
#include "utils/deformation_registry.hpp"

#include <vector>

class EdgeSmoothingParams : public IDeformationParams {
public:
    explicit EdgeSmoothingParams(float smoothing_factor, int iteration_count = 1)
        : smoothing_factor_(smoothing_factor),
          iteration_count_(iteration_count) {}

    id_t deformation_id() const noexcept override {
        return 2;
    }

    size_t hash() const noexcept override {
        return std::hash<float>()(smoothing_factor_) ^ std::hash<int>()(iteration_count_);
    }

    bool operator==(const IDeformationParams& other) const override {
        if (other.deformation_id() != deformation_id())
            return false;

        const EdgeSmoothingParams* other_params = dynamic_cast<const EdgeSmoothingParams*>(&other);

        return other_params && other_params->smoothing_factor_ == smoothing_factor_ &&
               other_params->iteration_count_ == iteration_count_;
    }

    float factor() const noexcept {
        return smoothing_factor_;
    }

    int iterations() const noexcept {
        return iteration_count_;
    }

private:
    float smoothing_factor_;
    int iteration_count_;
};

class EdgeSmoothingDeformation : public IDeformation {
public:
    id_t id() override {
        return 2;
    }

private:
    sptr<MatrixX3f> getDeformedVertices(sptr<const Mesh> mesh, const IDeformationParams& params,
                                        ThreadPool& thread_pool) override {
        const auto* smoothing_params = dynamic_cast<const EdgeSmoothingParams*>(&params);
        if (!smoothing_params) {
            std::cerr << "Invalid deformation params for EdgeSmoothingDeformation.\n";
            return nullptr;
        }

        float factor = smoothing_params->factor();
        int iteration_count = smoothing_params->iterations();

        auto vertices = mesh->vertices();
        auto vertex_neighbours = mesh->verticesNeighbours();
        uint64_t vertex_count = vertices->rows();

        auto deformed_vertices = std::make_shared<MatrixX3f>(*vertices);
        MatrixX3f vertices_delta = MatrixX3f::Zero(vertex_count, 3);

        auto num_threads = std::max(1U, thread_pool.workersCount());
        auto chunk_size = (vertex_count + num_threads - 1) / num_threads;

        for (int iter = 0; iter < iteration_count; ++iter) {
            vertices_delta.setZero();

            std::vector<std::future<void>> futures;
            for (int thread_index = 0; thread_index < num_threads; ++thread_index) {
                auto start_index = thread_index * chunk_size;
                auto end_index = std::min(start_index + chunk_size, vertex_count);

                futures.push_back(thread_pool.enqueue([&, start_index, end_index]() {
                    Eigen::RowVector3f neighbours_average;
                    for (size_t vertex_index = start_index; vertex_index < end_index; ++vertex_index) {
                        const auto& neighbours_list = (*vertex_neighbours)[vertex_index];
                        if (neighbours_list.empty())
                            continue;

                        neighbours_average.setZero();
                        for (int neighbour_index: neighbours_list) {
                            neighbours_average += deformed_vertices->row(neighbour_index);
                        }
                        neighbours_average /= static_cast<float>(neighbours_list.size());
                        vertices_delta.row(vertex_index) =
                                factor * (neighbours_average - deformed_vertices->row(vertex_index));
                    }
                }));
            }

            for (auto& future_task: futures) {
                future_task.get();
            }

            deformed_vertices->noalias() += vertices_delta;
        }

        return deformed_vertices;
    }
};

REGISTER_DEFORMATION(EdgeSmoothingDeformation, 2)
