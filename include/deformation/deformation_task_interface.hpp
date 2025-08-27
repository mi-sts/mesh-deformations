#pragma once

#include "structures/mesh.hpp"

class IDeformationTask {
public:
    virtual ~IDeformationTask() = default;

    // Returns the parent mesh if the task has not been completed, otherwise the function returns a deformed mesh.
    virtual mesh_ptr_t latest_mesh() const noexcept = 0;
    virtual bool is_completed() const noexcept = 0;
};
