#pragma once

#include "deformations_cache_interface.hpp"

#include <shared_mutex>
#include <future>
#include <deque>

class DeformationsSnapshot;
class Mesh;

class DeformationsCache : public IDeformationsCache {
public:
    void addSnapshotMesh(sptr<const Mesh> mesh, sptr<const DeformationsSnapshot> snapshot) override;
    void addBaseMesh(sptr<const Mesh> mesh) override;

    sptr<const DeformationsSnapshot> latestSnapshot(id_t mesh_family_id) const override;
    sptr<const Mesh> latestMesh(id_t mesh_family_id) const override;
    sptr<const Mesh> snapshotMesh(const DeformationsSnapshot& snapshot) const override;
    bool containsSnapshot(const DeformationsSnapshot &snapshot) const override;

private:
    // The snapshot hash to the mesh data mapping.
    std::unordered_map<size_t, MeshSnapshotData> meshes_data_;
    // The mesh family id to the deformation history.
    std::unordered_map<id_t, std::deque<sptr<const DeformationsSnapshot>>> meshes_deformations_;

    mutable std::shared_mutex meshes_mutex_;
};
