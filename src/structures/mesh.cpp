#include "structures/mesh.hpp"
#include "utils/data_types.hpp"

#include <Eigen/Sparse>
#include <igl/per_face_normals.h>
#include <igl/per_vertex_normals.h>

id_t Mesh::id() const noexcept {
    return id_;
}

id_t Mesh::family_id() const noexcept {
    return family_id_;
}

sptr<const MatrixX3f> Mesh::vertices() const noexcept {
    return vertices_;
}

sptr<const MatrixX3i> Mesh::faces() const noexcept {
    return faces_;
}

sptr<const MatrixX3f> Mesh::verticesNormals() noexcept {
    if (!vertices_normals_) {
        vertices_normals_ = calculateVerticesNormals(*vertices_, *faces_);
    }

    return vertices_normals_;
}

sptr<const MatrixX3f> Mesh::facesNormals() noexcept {
    if (!faces_normals_) {
        faces_normals_ = calculateFacesNormals(*vertices_, *faces_);
    }

    return faces_normals_;
}

sptr<std::vector<std::vector<int>>> Mesh::verticesNeighbours() const noexcept {
    return vertices_neighbours_;
}


bool Mesh::operator==(const Mesh& other) const noexcept {
    return id_ == other.id_;
}

id_t Mesh::getId() {
    return id_counter_++;
}

id_t Mesh::getFamilyId() {
    return family_id_counter_++;
}

sptr<MatrixX3f> Mesh::calculateVerticesNormals(const MatrixX3f& vertices, const MatrixX3i& triangles) {
    sptr<MatrixX3f> vertices_normals = std::make_shared<MatrixX3f>();
    igl::per_vertex_normals(vertices, triangles, *vertices_normals);

    return vertices_normals;
}

sptr<MatrixX3f> Mesh::calculateFacesNormals(const MatrixX3f& vertices, const MatrixX3i& triangles) {
    sptr<MatrixX3f> faces_normals = std::make_shared<MatrixX3f>();
    igl::per_face_normals(vertices, triangles, *faces_normals);

    return faces_normals;
}

sptr<std::vector<std::vector<int>>> Mesh::calculateVerticesNeighbours(const MatrixX3i& faces, uint64_t vertices_count) {
    auto neighbours = std::make_shared<std::vector<std::vector<int>>>(vertices_count);

    size_t face_count = faces.rows();
    for (size_t f = 0; f < face_count; ++f) {
        int v0 = faces(f, 0);
        int v1 = faces(f, 1);
        int v2 = faces(f, 2);

        (*neighbours)[v0].push_back(v1);
        (*neighbours)[v0].push_back(v2);

        (*neighbours)[v1].push_back(v0);
        (*neighbours)[v1].push_back(v2);

        (*neighbours)[v2].push_back(v0);
        (*neighbours)[v2].push_back(v1);
    }

    for (auto& neigh: *neighbours) {
        std::sort(neigh.begin(), neigh.end());
        neigh.erase(std::unique(neigh.begin(), neigh.end()), neigh.end());
    }

    return neighbours;
}
