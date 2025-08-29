#include "deformation/deformations_snapshot.hpp"

#include "deformation/deformation_params_interface.hpp"
#include "utils/hashes.hpp"

#include <deque>

id_t DeformationsSnapshot::mesh_id() const noexcept {
    return mesh_id_;
}

std::vector<sptr<IDeformationParams>> DeformationsSnapshot::deformations() const noexcept {
    const DeformationsSnapshot* current_snapshot = this;
    std::deque<const DeformationsSnapshot*> snapshots{this};
    while (current_snapshot->parent_snapshot_ != nullptr) {
        auto parent_snapshot = &(*current_snapshot->parent_snapshot_);
        snapshots.push_front(parent_snapshot);
        current_snapshot = parent_snapshot;
    }

    std::vector<sptr<IDeformationParams>> deformations;
    deformations.reserve(snapshots.size());
    for (int i = 0; i < snapshots.size(); ++i) {
        deformations[i] = snapshots[i]->current_deformation_;
    }

    return deformations;
}

size_t DeformationsSnapshot::hash() const noexcept {
    if (cached_hash_) {
        return *cached_hash_;
    }

    size_t hash = 0U;
    if (current_deformation_ == nullptr) {
        hash = std::hash<id_t>{}(mesh_id_);
    } else {
        hash = std::hash<id_t>{}(current_deformation_->deformation_id());
        hash = hashCombine(hash, current_deformation_->hash());

        if (parent_snapshot_) {
            hash = hashCombine(parent_snapshot_->hash(), hash);
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
