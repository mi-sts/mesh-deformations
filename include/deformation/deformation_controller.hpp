#pragma once

#include "cache/deformations_cache.hpp"
#include "deformation_controller_interface.hpp"
#include "threadpool/threadpool.hpp"

class Mesh;
class DeformationsSnapshot;

class DeformationController : public IDeformationController {
public:
    void addMesh(sptr<const Mesh> mesh) override;
    std::shared_future<MeshSnapshotData> applyDeformation(id_t mesh_family_id, sptr<const IDeformationParams> params) override;

    sptr<const Mesh> latestMesh(id_t mesh_family_id) const override;
    sptr<const DeformationsSnapshot> latestSnapshot(id_t mesh_family_id) const override;

    sptr<const Mesh> waitSnapshotMesh(const sptr<const DeformationsSnapshot>& snapshot) override;
    bool isMeshReady(const sptr<const DeformationsSnapshot>& snapshot) const override;

private:
    ThreadPool threadpool_;
    DeformationsCache cache_;

    // The snapshot hash to the mesh mapping.
    std::unordered_map<size_t, std::shared_future<MeshSnapshotData>> processing_meshes_data_;
    mutable std::shared_mutex processing_meshes_mutex_;
};
