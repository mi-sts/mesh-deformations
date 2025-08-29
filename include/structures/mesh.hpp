#pragma once

#include "utils/data_types.hpp"

#include <atomic>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Mesh {
public:
    Mesh(const MatrixX3f& vertices, const MatrixX3i& faces, const MatrixX3f& vertices_normals,
         const MatrixX3f& faces_normals)
        : id_(getId()),
          vertices_(std::make_shared<MatrixX3f>(vertices)),
          faces_(std::make_shared<MatrixX3i>(faces)),
          vertices_normals_(std::make_shared<MatrixX3f>(vertices_normals)),
          faces_normals_(std::make_shared<MatrixX3f>(faces_normals)),
          vertices_neighbours_(
                  std::make_shared<SparseMatrix<int>>(calculateVerticesNeighbours(faces, vertices.rows()))) {
        if (vertices.cols() != vertices_normals.cols()) {
            throw std::invalid_argument("The number of vertices normals does not match the number of vertices");
        }

        if (faces.cols() != faces_normals.cols()) {
            throw std::invalid_argument("The number of faces normals does not match the number of faces");
        }
    }

    Mesh(const MatrixX3f& vertices, const MatrixX3i& faces)
        : Mesh(vertices, faces, calculateVerticesNormals(vertices, faces), calculateFacesNormals(vertices, faces)) {}

    Mesh(const Mesh& other, const MatrixX3f& new_vertices) : id_(getId()) {
        faces_ = other.faces_;
        vertices_neighbours_ = other.vertices_neighbours_;

        vertices_ = std::make_shared<MatrixX3f>(new_vertices);
        vertices_normals_ = std::make_shared<MatrixX3f>(calculateVerticesNormals(new_vertices, *other.faces_));
        faces_normals_ = std::make_shared<MatrixX3f>(calculateFacesNormals(new_vertices, *other.faces_));
    }

    Mesh(const Mesh& other) : Mesh(other, *other.vertices_) {}

    id_t id() const noexcept;
    sptr<const MatrixX3f> vertices() const noexcept;
    sptr<const MatrixX3i> faces() const noexcept;
    sptr<const MatrixX3f> verticesNormals() const noexcept;
    sptr<const MatrixX3f> facesNormals() const noexcept;
    sptr<const SparseMatrix<int>> verticesNeighbours() const noexcept;

    bool operator==(const Mesh& other) const noexcept;

private:
    static id_t getId();
    static MatrixX3f calculateVerticesNormals(const MatrixX3f& vertices, const MatrixX3i& faces);
    static MatrixX3f calculateFacesNormals(const MatrixX3f& vertices, const MatrixX3i& faces);
    static SparseMatrix<int> calculateVerticesNeighbours(const MatrixX3i& faces, uint64_t vertices_count);

private:
    inline static id_t id_counter_{0u};

    sptr<MatrixX3f> vertices_;
    sptr<MatrixX3f> vertices_normals_;
    sptr<MatrixX3f> faces_normals_;

    // Shared among all cached meshes.
    sptr<MatrixX3i> faces_;
    sptr<SparseMatrix<int>> vertices_neighbours_;

    id_t id_;
};
