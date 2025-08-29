#include "deformation/deformation_controller.hpp"

#include "deformation/deformation_factory.hpp"
#include "deformation/deformations_snapshot.hpp"
#include "deformation/vertex_offset_deformation.hpp"
#include "structures/mesh.hpp"

std::shared_future<sptr<Mesh>> DeformationController::applyDeformation(id_t mesh_id, sptr<IDeformationParams> params) {
    sptr<DeformationsSnapshot> prev_snapshot = latestSnapshot(mesh_id);
    sptr<DeformationsSnapshot> new_snapshot = std::make_shared<DeformationsSnapshot>(mesh_id, params, prev_snapshot);

    auto future_mesh = threadpool_
                               .enqueue([this, params, prev_snapshot]() -> sptr<Mesh> {
                                   sptr<Mesh> parent_mesh = latestMesh(prev_snapshot->mesh_id());
                                   if (!parent_mesh) {
                                       return nullptr;
                                   }
                                   return VertexOffsetDeformation().applyDeformation(parent_mesh, *params);
                               })
                               .share();

    {
        std::lock_guard<std::mutex> lock(meshes_mutex_);
        meshes_[new_snapshot->hash()] = future_mesh;
        latest_snapshots_[mesh_id] = new_snapshot;
    }

    return future_mesh;
}


sptr<Mesh> DeformationController::latestMesh(id_t mesh_id) const {
    auto snapshot = latestSnapshot(mesh_id);
    if (!snapshot) {
        return nullptr;
    }
    return snapshotMesh(snapshot);
}

sptr<DeformationsSnapshot> DeformationController::latestSnapshot(id_t mesh_id) const {
    std::lock_guard<std::mutex> lock(meshes_mutex_);
    auto snapshot_it = latest_snapshots_.find(mesh_id);
    if (snapshot_it == latest_snapshots_.end()) {
        return nullptr;
    }

    return snapshot_it->second;
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
void DeformationController::addMesh(sptr<Mesh> mesh) {
    if (!mesh) return;

    auto snapshot = std::make_shared<DeformationsSnapshot>(mesh->id(), nullptr, nullptr);

    {
        std::lock_guard<std::mutex> lock(meshes_mutex_);

        std::promise<sptr<Mesh>> promise;
        promise.set_value(mesh);
        meshes_[snapshot->hash()] = promise.get_future().share();

        latest_snapshots_[mesh->id()] = snapshot;
    }
}
