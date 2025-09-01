#pragma once

#include "utils/data_types.hpp"

#include <atomic>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Mesh {
public:
    Mesh(const MatrixX3f& vertices, const MatrixX3i& faces)
        : id_(getId()),
          family_id_(getFamilyId()),
          vertices_(std::make_shared<MatrixX3f>(vertices)),
          faces_(std::make_shared<MatrixX3i>(faces)),
          vertices_neighbours_(calculateVerticesNeighbours(faces, vertices.rows())) {}

    Mesh(const MatrixX3f& vertices, const MatrixX3i& faces, const MatrixX3f& vertices_normals,
         const MatrixX3f& faces_normals)
        : Mesh(vertices, faces) {
            vertices_normals_ = std::make_shared<MatrixX3f>(vertices_normals);
            faces_normals_ = std::make_shared<MatrixX3f>(faces_normals);

        if (vertices.cols() != vertices_normals.cols()) {
            throw std::invalid_argument("The number of vertices normals does not match the number of vertices");
        }

        if (faces.cols() != faces_normals.cols()) {
            throw std::invalid_argument("The number of faces normals does not match the number of faces");
        }
    }

    Mesh(const Mesh& other, sptr<MatrixX3f> new_vertices)
        : id_(getId()),
          family_id_(other.family_id_),
          faces_(other.faces_),
          vertices_neighbours_(other.vertices_neighbours_),
          vertices_(std::move(new_vertices)) {}

    ID_t id() const noexcept;
    ID_t family_id() const noexcept;
    sptr<const MatrixX3f> vertices() const noexcept;
    sptr<const MatrixX3i> faces() const noexcept;
    sptr<const MatrixX3f> verticesNormals() noexcept;
    sptr<const MatrixX3f> facesNormals() noexcept;
    sptr<std::vector<std::vector<int>>> verticesNeighbours() const noexcept;

    bool operator==(const Mesh& other) const noexcept;

private:
    static ID_t getId();
    static ID_t getFamilyId();
    static sptr<MatrixX3f> calculateVerticesNormals(const MatrixX3f& vertices, const MatrixX3i& faces);
    static sptr<MatrixX3f> calculateFacesNormals(const MatrixX3f& vertices, const MatrixX3i& faces);
    static sptr<std::vector<std::vector<int>>> calculateVerticesNeighbours(const MatrixX3i& faces, uint64_t vertices_count);

private:
    inline static ID_t id_counter_{0u};
    inline static ID_t family_id_counter_{0u};

    sptr<MatrixX3f> vertices_;
    sptr<MatrixX3f> vertices_normals_;
    sptr<MatrixX3f> faces_normals_;

    // Shared among all cached meshes.
    sptr<MatrixX3i> faces_;
    sptr<std::vector<std::vector<int>>> vertices_neighbours_;

    ID_t id_;
    ID_t family_id_;
};
