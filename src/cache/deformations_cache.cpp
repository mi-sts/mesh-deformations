#include "cache/deformations_cache.hpp"
#include <mutex>

void DeformationsCache::add(const sptr<DeformationsSnapshot>& snapshot) {
    std::unique_lock lock(snapshots_mutex_);
    snapshots_[snapshot->hash()] = snapshot;
}

sptr<DeformationsSnapshot> DeformationsCache::get(size_t hash) const noexcept {
    std::shared_lock lock(snapshots_mutex_);
    auto it = snapshots_.find(hash);
    return it != snapshots_.end() ? it->second : nullptr;
}

bool DeformationsCache::contains(size_t hash) const noexcept {
    std::shared_lock lock(snapshots_mutex_);
    return snapshots_.contains(hash);
}
