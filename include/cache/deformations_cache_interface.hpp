#pragma once

#include "deformation/deformations_snapshot.hpp"

class IDeformationsCache {
public:
    virtual ~IDeformationsCache() = default;

    virtual void add(const sptr<DeformationsSnapshot>& snapshot) = 0;
    virtual sptr<DeformationsSnapshot> get(size_t hash) const noexcept = 0;
    virtual bool contains(size_t hash) const noexcept = 0;
};
