#pragma once

#include "deformation_interface.hpp"
#include "deformation_params_interface.hpp"
#include "structures/mesh.hpp"
#include "utils/data_types.hpp"
#include "utils/hashes.hpp"
#include "utils/deformation_registry.hpp"

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

    sptr<MatrixX3f> getDeformedVertices(sptr<const Mesh> mesh, const IDeformationParams& params) override {
        auto& offset_params = dynamic_cast<const VertexOffsetParams&>(params);

        sptr<MatrixX3f> vertices = std::make_shared<MatrixX3f>(*mesh->vertices());
        *vertices += offset_params.offsets();

        return vertices;
    }
};

REGISTER_DEFORMATION(VertexOffsetDeformation, 1)
