#pragma once

#include "deformation/deformation_params_interface.hpp"
#include "structures/mesh.hpp"

class ThreadPool;

class IDeformation {
public:
    virtual ~IDeformation() = default;

    virtual ID_t id() = 0;
    sptr<Mesh> applyDeformation(sptr<const Mesh> mesh, const IDeformationParams& params, ThreadPool& threadpool) const {
        auto deformed_vertices = getDeformedVertices(mesh, params, threadpool);

        return std::make_shared<Mesh>(*mesh, deformed_vertices);
    }

    virtual sptr<MatrixX3f> getDeformedVertices(sptr<const Mesh> mesh, const IDeformationParams& params,
                                                ThreadPool& threadpool) const = 0;
};
