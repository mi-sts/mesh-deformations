#pragma once

#include "cache/deformations_cache.hpp"
#include "deformation_controller_interface.hpp"
#include "threadpool/threadpool.hpp"

class Mesh;
class DeformationsSnapshot;

class DeformationController : public IDeformationController {
public:
    std::shared_future<sptr<Mesh>> applyDeformation(id_t mesh_id, sptr<IDeformationParams> params) override;

    void addMesh(sptr<Mesh> mesh);
    sptr<Mesh> latestMesh(id_t mesh_id) const override;
    sptr<DeformationsSnapshot> latestSnapshot(id_t mesh_id) const override;

    sptr<Mesh> snapshotMesh(const sptr<DeformationsSnapshot>& snapshot) const override;
    sptr<Mesh> waitSnapshotMesh(const sptr<DeformationsSnapshot>& snapshot) override;
    bool isMeshReady(const sptr<DeformationsSnapshot>& snapshot) const override;

private:
    ThreadPool threadpool_;
    DeformationsCache cache_;

    std::unordered_map<id_t, sptr<DeformationsSnapshot>> latest_snapshots_;
    mutable std::unordered_map<size_t, std::shared_future<sptr<Mesh>>> meshes_;

    mutable std::mutex meshes_mutex_;
};
