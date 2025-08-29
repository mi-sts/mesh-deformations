#pragma once

#include "deformation/deformation_params_interface.hpp"
#include "structures/mesh.hpp"

class IDeformation {
public:
    virtual ~IDeformation() = default;

    virtual id_t id() = 0;
    sptr<Mesh> applyDeformation(sptr<const Mesh> mesh, const IDeformationParams& params) {
        auto deformed_vertices = getDeformedVertices(mesh, params);

        return std::make_shared<Mesh>(*mesh, *deformed_vertices);
    }

private:
    virtual sptr<MatrixX3f> getDeformedVertices(sptr<const Mesh> mesh, const IDeformationParams& params) = 0;
};
