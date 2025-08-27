#pragma once

#include "deformation_params_interface.hpp"
#include "utils/data_types.hpp"

#include <optional>
#include <utility>

class DeformationsSnapshot {
public:
    explicit DeformationsSnapshot(id_t mesh_id, sptr<IDeformationParams> new_params = nullptr,
                                  sptr<DeformationsSnapshot> parent_snapshot = nullptr)
        : mesh_id_(mesh_id),
          current_deformation_(std::move(new_params)),
          parent_snapshot_(std::move(parent_snapshot)) {}

    id_t mesh_id() const noexcept;
    const std::vector<IDeformationParams>& deformations() const noexcept;

    size_t hash() const noexcept;
    bool operator==(const DeformationsSnapshot& other) const noexcept;

private:
    id_t mesh_id_;
    mutable std::optional<size_t> cached_hash_;
    sptr<DeformationsSnapshot> parent_snapshot_;
    sptr<IDeformationParams> current_deformation_;
};
