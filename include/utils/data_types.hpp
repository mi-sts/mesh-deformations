#pragma once

#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/src/Core/Matrix.h>
#include <cstdint>
#include <memory>

class Mesh;
class DeformationsSnapshot;

using id_t = uint64_t;

template <typename T>
using uptr = std::unique_ptr<T>;
template <typename T>
using sptr = std::shared_ptr<T>;

using Vector3f = Eigen::Vector3f;
using MatrixX3f = Eigen::Matrix<float, Eigen::Dynamic, 3>;
using MatrixX3i = Eigen::Matrix<int, Eigen::Dynamic, 3>;
template <typename Type>
using SparseMatrix = Eigen::SparseMatrix<Type, Eigen::RowMajor>;

struct MeshSnapshotData
{
    sptr<const Mesh> mesh;
    sptr<const DeformationsSnapshot> snapshot;
};
