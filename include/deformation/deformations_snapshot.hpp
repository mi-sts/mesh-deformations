#pragma once

#include "utils/data_types.hpp"

#include <optional>
#include <utility>

class IDeformationParams;

class DeformationsSnapshot {
public:
    explicit DeformationsSnapshot(ID_t mesh_id, sptr<const IDeformationParams> new_params = nullptr,
                                  sptr<const DeformationsSnapshot> parent_snapshot = nullptr)
        : mesh_id_(mesh_id),
          current_deformation_(std::move(new_params)),
          parent_snapshot_(std::move(parent_snapshot)) {
    }

    ID_t mesh_id() const noexcept;
    std::vector<sptr<IDeformationParams>> deformations() const noexcept;

    size_t hash() const noexcept;
    bool operator==(const DeformationsSnapshot& other) const noexcept;

private:
    ID_t mesh_id_;
    mutable std::optional<size_t> cached_hash_;
    sptr<const DeformationsSnapshot> parent_snapshot_;
    sptr<const IDeformationParams> current_deformation_;
};
