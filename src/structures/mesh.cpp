#include "structures/mesh.hpp"

#include <igl/per_face_normals.h>
#include <igl/per_vertex_normals.h>
#include <Eigen/Sparse>

id_t Mesh::id() const noexcept {
    return id_;
}

sptr<const MatrixX3f> Mesh::vertices() const noexcept {
    return vertices_;
}

sptr<const MatrixX3i> Mesh::faces() const noexcept {
    return faces_;
}

bool Mesh::operator==(const Mesh& other) const noexcept {
    return id_ == other.id_;
}

id_t Mesh::getId() {
    return id_counter_++;
}

MatrixX3f Mesh::calculateVerticesNormals(const MatrixX3f& vertices, const MatrixX3i& triangles) {
    MatrixX3f vertices_normals;
    igl::per_vertex_normals(vertices, triangles, vertices_normals);

    return vertices_normals;
}

MatrixX3f Mesh::calculateFacesNormals(const MatrixX3f& vertices, const MatrixX3i& triangles) {
    MatrixX3f faces_normals;
    igl::per_face_normals(vertices, triangles, faces_normals);

    return faces_normals;
}

SparseMatrix<int> Mesh::calculateVerticesNeighbours(const MatrixX3i& faces, uint64_t vertices_count) {
    std::vector<Eigen::Triplet<int>> faces_triplets;
    faces_triplets.reserve(faces.rows() * 6);

    for (int i = 0; i < faces.rows(); ++i) {
        int v0 = faces(i, 0);
        int v1 = faces(i, 1);
        int v2 = faces(i, 2);

        faces_triplets.emplace_back(v0, v1, 1);
        faces_triplets.emplace_back(v1, v0, 1);

        faces_triplets.emplace_back(v1, v2, 1);
        faces_triplets.emplace_back(v2, v1, 1);

        faces_triplets.emplace_back(v2, v0, 1);
        faces_triplets.emplace_back(v0, v2, 1);
    }

    SparseMatrix<int> vertices_neighbours(vertices_count, vertices_count);
    vertices_neighbours.setFromTriplets(faces_triplets.begin(), faces_triplets.end());

    return vertices_neighbours;
}
