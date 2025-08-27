#pragma once

#include "structures/mesh.hpp"

#include <concepts>

template <std::unsigned_integral T = size_t>
static size_t hash_combine(T a, T b) noexcept {
    return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
}
