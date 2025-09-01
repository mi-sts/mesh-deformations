#include "deformation/deformation_controller.hpp"

#include "deformation/deformation_factory.hpp"
#include "deformation/deformations_snapshot.hpp"
#include "structures/mesh.hpp"
#include "utils/containers.hpp"

std::shared_future<MeshSnapshotData> DeformationController::applyDeformation(id_t mesh_family_id,
                                                                             sptr<const IDeformationParams> params) {
    sptr<const DeformationsSnapshot> prev_snapshot = latestSnapshot(mesh_family_id);
    sptr<DeformationsSnapshot> new_snapshot =
            std::make_shared<DeformationsSnapshot>(mesh_family_id, params, prev_snapshot);

    auto mesh_future =
            threadpool_
                    .enqueue([this, mesh_family_id, params, prev_snapshot, new_snapshot]() -> MeshSnapshotData {
                        sptr<const Mesh> parent_mesh = latestMesh(mesh_family_id);
                        if (!parent_mesh) {
                            std::cerr << "There is no mesh for the given mesh family id.\n";
                            return MeshSnapshotData{};
                        }

                        auto future_mesh = DeformationFactory::create(params->deformation_id())
                                                   ->applyDeformation(parent_mesh, *params, threadpool_);
                        cache_.addSnapshotMesh(future_mesh, new_snapshot);
                        return MeshSnapshotData{future_mesh, new_snapshot};
                    })
                    .share();

    std::lock_guard lock(processing_meshes_mutex_);
    processing_meshes_data_[new_snapshot->hash()] = mesh_future;

    return mesh_future;
}


sptr<const Mesh> DeformationController::latestMesh(id_t mesh_family_id) const {
    return cache_.latestMesh(mesh_family_id);
}

sptr<const DeformationsSnapshot> DeformationController::latestSnapshot(id_t mesh_family_id) const {
    return cache_.latestSnapshot(mesh_family_id);
}

sptr<const Mesh> DeformationController::waitSnapshotMesh(const sptr<const DeformationsSnapshot>& snapshot) {
    if (!snapshot) {
        return nullptr;
    }

    if (cache_.containsSnapshot(*snapshot)) {
        return cache_.snapshotMesh(*snapshot);
    }

    std::shared_future<MeshSnapshotData> mesh_data_future;
    {
        std::shared_lock lock(processing_meshes_mutex_);
        auto it = processing_meshes_data_.find(snapshot->hash());
        if (it == processing_meshes_data_.end()) {
            std::cerr << "There is no mesh for the given snapshot to wait.\n";
            return nullptr;
        }
        mesh_data_future = it->second;
    }

    return mesh_data_future.get().mesh;
}

bool DeformationController::isMeshReady(const sptr<const DeformationsSnapshot>& snapshot) const {
    if (!snapshot) {
        return false;
    }

    if (cache_.containsSnapshot(*snapshot)) {
        return true;
    }

    std::shared_lock lock(processing_meshes_mutex_);
    return contains(processing_meshes_data_, snapshot->hash());
}

void DeformationController::addMesh(sptr<const Mesh> mesh) {
    if (!mesh) {
        std::cerr << "The given mesh is invalid.\n";
        return;
    }

    cache_.addBaseMesh(mesh);
}
