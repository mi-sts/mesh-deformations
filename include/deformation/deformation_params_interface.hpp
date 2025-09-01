#pragma once

#include "utils/data_types.hpp"

#include <cstddef>

class IDeformationParams {
public:
    virtual ~IDeformationParams() = default;

    virtual ID_t deformation_id() const noexcept = 0;
    virtual size_t hash() const noexcept = 0;
    virtual bool operator==(const IDeformationParams& other) const = 0;
};
