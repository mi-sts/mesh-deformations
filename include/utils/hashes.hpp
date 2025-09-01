#pragma once

#include "structures/mesh.hpp"

#include <concepts>
#include <xxhash.h>

static size_t hashCombine(size_t a, size_t b) noexcept {
    return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
}

template <typename Matrix>
static std::size_t matrixHash(const Matrix& matrix) {
    const void* matrix_data = matrix.data();
    const std::size_t matrix_size = sizeof(typename Matrix::Scalar) * matrix.size();

    return static_cast<std::size_t>(XXH3_64bits(matrix_data, matrix_size));
}
