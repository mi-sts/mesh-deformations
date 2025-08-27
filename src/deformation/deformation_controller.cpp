#include "deformation/deformation_controller.hpp"

std::future<sptr<DeformationsSnapshot>> DeformationController::applyDeformation(id_t mesh_id,
                                                                                sptr<IDeformationParams> params) {
}


sptr<Mesh> DeformationController::latestMesh(id_t mesh_id) const {
    auto snapshot = latestSnapshot(mesh_id);
    if (!snapshot) {
        return nullptr;
    }
    return snapshotMesh(snapshot);
}

sptr<DeformationsSnapshot> DeformationController::latestSnapshot(id_t mesh_id) const {
    auto snapshot_it = latest_snapshots_.find(mesh_id);
    if (snapshot_it != latest_snapshots_.end()) {
        return snapshot_it->second;
    }

    return nullptr;
}

sptr<Mesh> DeformationController::snapshotMesh(const sptr<DeformationsSnapshot>& snapshot) const {
    if (!snapshot) {
        return nullptr;
    }

    auto snapshot_hash = snapshot->hash();
    auto mesh_it = meshes_.find(snapshot_hash);
    if (mesh_it == meshes_.end()) {
        return nullptr;
    }

    auto& mesh_future = mesh_it->second;
    if (mesh_future.valid() && mesh_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        return mesh_future.get();
    }

    return nullptr;
}

sptr<Mesh> DeformationController::waitSnapshotMesh(const sptr<DeformationsSnapshot>& snapshot) {
    if (!snapshot) {
        return nullptr;
    }

    auto snapshot_hash = snapshot->hash();
    auto it = meshes_.find(snapshot_hash);
    if (it == meshes_.end()) {
        return nullptr;
    }

    return it->second.get();
}

bool DeformationController::isMeshReady(const sptr<DeformationsSnapshot>& snapshot) const {
    if (!snapshot) {
        return false;
    }

    return snapshotMesh(snapshot) != nullptr;
}
