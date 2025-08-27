#pragma once

#include "utils/data_types.hpp"

#include <future>

class DeformationsSnapshot;
class IDeformationAlgorithm;
class IDeformationParams;

class IDeformationController {
public:
    virtual ~IDeformationController() = default;

    virtual std::future<sptr<DeformationsSnapshot>> applyDeformation(id_t mesh_id, sptr<IDeformationParams> new_deformation) = 0;
    virtual sptr<DeformationsSnapshot> latestSnapshot(id_t mesh_id) const = 0;
    virtual sptr<Mesh> latestMesh(id_t mesh_id) const = 0;

    virtual sptr<Mesh> snapshotMesh(const sptr<DeformationsSnapshot>& snapshot) const = 0;
    virtual bool isMeshReady(const sptr<DeformationsSnapshot>& snapshot) const = 0;
    virtual sptr<Mesh> waitSnapshotMesh(const sptr<DeformationsSnapshot>& snapshot) = 0;
};
