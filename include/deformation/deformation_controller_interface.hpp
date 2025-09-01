#pragma once

#include "utils/data_types.hpp"

#include <future>

class DeformationsSnapshot;
class Mesh;
class IDeformationParams;

class IDeformationController {
public:
    virtual ~IDeformationController() = default;

    virtual void addMesh(sptr<const Mesh> mesh) = 0;
    virtual std::shared_future<MeshSnapshotData> applyDeformation(ID_t mesh_family_id,
                                                                  sptr<const IDeformationParams> params) = 0;

    virtual sptr<const Mesh> latestMesh(ID_t mesh_family_id) const = 0;
    virtual sptr<const DeformationsSnapshot> latestSnapshot(ID_t mesh_family_id) const = 0;

    virtual sptr<const Mesh> waitSnapshotMesh(const sptr<const DeformationsSnapshot>& snapshot) = 0;
    virtual bool isMeshReady(const sptr<const DeformationsSnapshot>& snapshot) const = 0;
};
