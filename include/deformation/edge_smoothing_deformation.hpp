#pragma once

#include "deformation_interface.hpp"
#include "deformation_params_interface.hpp"
#include "utils/deformation_registry.hpp"

class EdgeSmoothingParams : public IDeformationParams {
public:
    EdgeSmoothingParams(float smoothing_factor) : factor_(smoothing_factor) {}

    id_t deformation_id() const noexcept override {
        return 2;
    }

    size_t hash() const noexcept override {
        return std::hash<float>()(factor_);
    }

    bool operator==(const IDeformationParams& other) const override {
        if (other.deformation_id() != deformation_id())
            return false;
        const EdgeSmoothingParams* o = dynamic_cast<const EdgeSmoothingParams*>(&other);
        return o && o->factor_ == factor_;
    }

    float factor() const {
        return factor_;
    }

private:
    float factor_;
};

class EdgeSmoothingDeformation : public IDeformation {
public:
    id_t id() override { return 2; }

private:
    sptr<MatrixX3f> getDeformedVertices(sptr<const Mesh> mesh, const IDeformationParams& params) override {
        auto smoothing_params = dynamic_cast<const EdgeSmoothingParams*>(&params);
        if (!smoothing_params) throw std::runtime_error("Invalid params for EdgeSmoothingDeformation");

        const float alpha = smoothing_params->factor();
        sptr<const MatrixX3f> vertices = mesh->vertices();
        sptr<MatrixX3f> new_vertices = std::make_shared<MatrixX3f>(*mesh->vertices());

        for (int i = 0; i < vertices->rows(); ++i) {
            Eigen::SparseVector<int> neighbors = mesh->verticesNeighbours()->row(i);

            Vector3f avg = Vector3f::Zero();
            for (Eigen::SparseVector<int>::InnerIterator it(neighbors); it; ++it) {
                avg += vertices->row(it.index());
            }

            avg /= static_cast<float>(neighbors.size());

            new_vertices->row(i) = (1.0f - alpha) * vertices->row(i) + alpha * avg.transpose();
        }

        return new_vertices;
    }
};

REGISTER_DEFORMATION(EdgeSmoothingDeformation, 2)
