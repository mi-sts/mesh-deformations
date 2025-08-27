#pragma once

#include "structures/mesh.hpp"

class IDeformationParams;

class IDeformation {
public:
    virtual ~IDeformation() = default;

    virtual id_t id() = 0;
    virtual mesh_ptr_t applyDeformation(const mesh_ptr_t mesh, IDeformationParams& params) = 0;
};
