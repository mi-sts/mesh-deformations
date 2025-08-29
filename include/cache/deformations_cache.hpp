#pragma once

#include "deformations_cache_interface.hpp"

#include <shared_mutex>

class DeformationSnapshot;

class DeformationsCache : public IDeformationsCache {
public:
    void add(const sptr<DeformationsSnapshot>& snapshot) override;
    sptr<DeformationsSnapshot> get(size_t hash) const noexcept override;
    bool contains(size_t hash) const noexcept override;

private:
    mutable std::shared_mutex snapshots_mutex_;
    std::unordered_map<size_t, sptr<DeformationsSnapshot>> snapshots_;
};
