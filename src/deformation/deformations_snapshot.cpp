#pragma once

#include "deformation/deformations_snapshot.hpp"
#include "utils/hashes.hpp"

size_t DeformationsSnapshot::hash() const noexcept {
    if (cached_hash_) {
        return *cached_hash_;
    }

    size_t hash = 0U;
    if (current_deformation_ == nullptr) {
        hash = std::hash<id_t>{}(mesh_id_);
    } else {
        hash = std::hash<id_t>{}(current_deformation_->deformation_id());
        hash = hash_combine(hash, current_deformation_->hash());

        if (parent_snapshot_) {
            hash = hash_combine(parent_snapshot_->hash(), hash);
        }
        cached_hash_ = hash;
    }

    return hash;
}

bool DeformationsSnapshot::operator==(const DeformationsSnapshot& other) const noexcept {
    bool are_parents_equal =
            (parent_snapshot_ && other.parent_snapshot_ && *parent_snapshot_ == *other.parent_snapshot_) ||
            (parent_snapshot_ == nullptr && other.parent_snapshot_ == nullptr);

    bool are_deformations_equal = (current_deformation_ && other.current_deformation_ &&
                                   *current_deformation_ == *other.current_deformation_) ||
                                  (current_deformation_ == nullptr && other.current_deformation_ == nullptr);

    return mesh_id_ == other.mesh_id_ && are_deformations_equal && are_parents_equal;
}
