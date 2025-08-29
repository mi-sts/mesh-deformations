#pragma once

#include "utils/data_types.hpp"

class DeformationsSnapshot;
class Mesh;

class IDeformationsCache {
public:
    virtual ~IDeformationsCache() = default;

    virtual void addSnapshotMesh(sptr<const Mesh> mesh, sptr<const DeformationsSnapshot> snapshot) = 0;
    virtual void addBaseMesh(sptr<const Mesh> mesh) = 0;

    virtual sptr<const Mesh> latestMesh(id_t mesh_family_id) const = 0;
    virtual sptr<const DeformationsSnapshot> latestSnapshot(id_t mesh_family_id) const = 0;
    virtual sptr<const Mesh> snapshotMesh(const DeformationsSnapshot& snapshot) const = 0;

    virtual bool containsSnapshot(const DeformationsSnapshot& snapshot) const = 0;
};
