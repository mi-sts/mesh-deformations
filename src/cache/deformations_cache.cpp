#include "cache/deformations_cache.hpp"

#include "deformation/deformations_snapshot.hpp"
#include "structures/mesh.hpp"
#include "utils/containers.hpp"

#include <deque>
#include <iostream>
#include <mutex>

void DeformationsCache::addSnapshotMesh(sptr<const Mesh> mesh, sptr<const DeformationsSnapshot> snapshot) {
    if (snapshot == nullptr) {
        std::cerr << "The adding snapshot is invalid.\n";
    }

    std::lock_guard<std::shared_mutex> lock(meshes_mutex_);

    meshes_data_[snapshot->hash()] = MeshSnapshotData{mesh, snapshot};
    if (!contains(meshes_deformations_, mesh->family_id())) {
        meshes_deformations_[mesh->family_id()] = std::deque<sptr<const DeformationsSnapshot>>();
    }
    meshes_deformations_[mesh->family_id()].push_back(snapshot);
}

void DeformationsCache::addBaseMesh(sptr<const Mesh> mesh) {
    {
        std::lock_guard<std::shared_mutex> lock(meshes_mutex_);

        if (contains(meshes_deformations_, mesh->family_id())) {
            std::cerr << "Base mesh for the given family id is already exist.\n";
            return;
        }
    }

    auto base_snapshot = std::make_shared<DeformationsSnapshot>(mesh->id());
    addSnapshotMesh(mesh, base_snapshot);
}

sptr<const DeformationsSnapshot> DeformationsCache::latestSnapshot(id_t mesh_family_id) const {
    std::shared_lock<std::shared_mutex> lock(meshes_mutex_);

    if (!contains(meshes_deformations_, mesh_family_id)) {
        std::cerr << "There is no mashes for the given family id.\n";
        return nullptr;
    }

    return meshes_deformations_.at(mesh_family_id).back();
}

sptr<const Mesh> DeformationsCache::latestMesh(id_t mesh_family_id) const {
    auto latest_snapshot = latestSnapshot(mesh_family_id);
    if (!contains(meshes_data_, latest_snapshot->hash())) {
        std::cerr << "There is no mesh with the given hash.\n";
        return nullptr;
    }

    return meshes_data_.at(latest_snapshot->hash()).mesh;
}

sptr<const Mesh> DeformationsCache::snapshotMesh(const DeformationsSnapshot& snapshot) const {
    std::shared_lock<std::shared_mutex> lock(meshes_mutex_);
    if (!containsSnapshot(snapshot)) {
        std::cerr << "There is no mashes with the given snapshot.\n";
        return nullptr;
    }

    return meshes_data_.at(snapshot.hash()).mesh;
}

bool DeformationsCache::containsSnapshot(const DeformationsSnapshot& snapshot) const {
    return contains(meshes_data_, snapshot.hash());
}
