#pragma once

#include "deformation_interface.hpp"
#include "deformation_params_interface.hpp"
#include "structures/mesh.hpp"
#include "utils/data_types.hpp"
#include "utils/deformation_registry.hpp"
#include "utils/hashes.hpp"

#include <vector>

class VertexOffsetParams : public IDeformationParams {
public:
    VertexOffsetParams(const MatrixX3f& offsets) : offsets_(offsets) {}

    id_t deformation_id() const noexcept override {
        return 1;
    }

    size_t hash() const noexcept override {
        return matrixHash(offsets_);
    }

    bool operator==(const IDeformationParams& other) const override {
        if (other.deformation_id() != deformation_id())
            return false;

        const VertexOffsetParams* o = dynamic_cast<const VertexOffsetParams*>(&other);

        return o && o->offsets_ == offsets_;
    }

    const MatrixX3f& offsets() const {
        return offsets_;
    }

private:
    const MatrixX3f& offsets_;
};

class VertexOffsetDeformation : public IDeformation {
public:
    id_t id() override {
        return 1;
    }

    sptr<MatrixX3f> getDeformedVertices(sptr<const Mesh> mesh, const IDeformationParams& params,
                                        ThreadPool& threadpool) override {
        const VertexOffsetParams* offset_params = dynamic_cast<const VertexOffsetParams*>(&params);
        if (!offset_params) {
            std::cerr << "Invalid deformation params for VertexOffsetDeformation.\n";
            return nullptr;
        }

        auto vertices = std::make_shared<MatrixX3f>(*mesh->vertices());
        size_t vertex_count = vertices->rows();

        threadpool
                .enqueue([&, vertex_count]() {
                    for (size_t i = 0; i < vertex_count; ++i) {
                        vertices->row(i) += offset_params->offsets().row(i);
                    }
                })
                .get();

        return vertices;
    }
};

REGISTER_DEFORMATION(VertexOffsetDeformation, 1)
