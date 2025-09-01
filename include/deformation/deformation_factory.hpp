#pragma once

#include "deformation_interface.hpp"
#include "utils/data_types.hpp"

#include <mutex>

class DeformationFactory {
public:
    static void registerDeformation(id_t id, sptr<IDeformation> deformation) {
        std::lock_guard<std::mutex> lock(mutex_);
        deformations_[id] = std::move(deformation);
    }

    static sptr<IDeformation> create(id_t id) {
        auto it = deformations_.find(id);
        if (it == deformations_.end()) {
            throw std::runtime_error("Deformation ID is not found");
        }
        return it->second;
    }

private:
    inline static std::unordered_map<id_t, sptr<IDeformation>> deformations_;
    inline static std::mutex mutex_;
};
