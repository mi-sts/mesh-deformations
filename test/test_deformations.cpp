#include <catch2/catch_test_macros.hpp>
#include "deformation/vertex_offset_deformation.hpp"
#include "deformation/edge_smoothing_deformation.hpp"
#include "structures/mesh.hpp"
#include "threadpool/threadpool.hpp"

TEST_CASE("Vertex offset deformation applies correct offsets", "[deformation]") {
    MatrixX3f vertices(3, 3);
    vertices << 0, 0, 0,
            1, 1, 1,
            2, 2, 2;

    MatrixX3i faces(1, 3);
    faces << 0, 1, 2;

    auto mesh_ptr = std::make_shared<Mesh>(vertices, faces);

    MatrixX3f offsets(3, 3);
    offsets << 1, 1, 1,
            1, 1, 1,
            1, 1, 1;

    VertexOffsetParams params(offsets);
    VertexOffsetDeformation deformation;
    ThreadPool pool(1);

    auto deformed = deformation.getDeformedVertices(mesh_ptr, params, pool);

    REQUIRE(deformed != nullptr);
    REQUIRE(deformed->row(0).isApprox((Eigen::RowVector3f() << 1.0f, 1.0f, 1.0f).finished()));
    REQUIRE(deformed->row(1).isApprox((Eigen::RowVector3f() << 2.0f, 2.0f, 2.0f).finished()));
    REQUIRE(deformed->row(2).isApprox((Eigen::RowVector3f() << 3.0f, 3.0f, 3.0f).finished()));
}

TEST_CASE("Edge smoothing deformation moves vertices closer to neighbours", "[deformation]") {
    MatrixX3f vertices(3, 3);
    vertices << 0, 0, 0,
            2, 0, 0,
            1, 2, 0;

    MatrixX3i faces(1, 3);
    faces << 0, 1, 2;

    auto mesh_ptr = std::make_shared<Mesh>(vertices, faces);

    EdgeSmoothingParams params(0.5f, 1);
    EdgeSmoothingDeformation deformation;
    ThreadPool pool(2);

    auto deformed = deformation.getDeformedVertices(mesh_ptr, params, pool);

    REQUIRE(deformed != nullptr);
    REQUIRE_FALSE(deformed->isApprox(vertices));

    Eigen::RowVector3f centroid = ((Eigen::RowVector3f() << 1.0f, 2.0f/3.0f, 0.0f).finished());

    for (int i = 0; i < 3; ++i) {
        auto original_dist = (vertices.row(i) - centroid).norm();
        auto new_dist = (deformed->row(i) - centroid).norm();
        REQUIRE(new_dist < original_dist + 1e-6f);
    }
}
