#include "deformation/deformation_controller.hpp"
#include "deformation/edge_smoothing_deformation.hpp"
#include "deformation/vertex_offset_deformation.hpp"
#include "igl/opengl/glfw/Viewer.h"
#include "igl/readOBJ.h"
#include "utils/mesh_loading.hpp"
#include "utils/data_types.hpp"
#include "utils/filesystem.hpp"
#include "utils/mutithreading.hpp"

#include <iostream>

int main() {
        std::cout << "This executable is only for demonstration of deformation algorithms work.\n";
        std::cout << "For benchmarking use mesh-deformations-benchmark executable.\n";
        std::cout << "IGL view has some freezes, that have nothing to do with the deformation system.\n";
        std::cout << "Press SPACE to apply the deformation (they alternate).\n";

        auto model_path = (getExecutablePath() / "models" / "cow.obj").string();
        auto mesh = loadObjMesh(model_path);

        auto deformation_controller = std::make_shared<DeformationController>();
        deformation_controller->addMesh(mesh);

        const auto& mesh_vertices = *mesh->vertices();
        auto vertex_offset_matrix = std::make_shared<MatrixX3f>(mesh_vertices);
        for (int i = 0; i < mesh_vertices.rows(); ++i) {
            for (int j = 0; j < mesh_vertices.row(i).size(); ++j) {
                (*vertex_offset_matrix)(i, j) = 0.05f * ((rand() % 200) / 100.f - 1.f);
            }
        }
        auto vertex_offset_params = std::make_shared<VertexOffsetParams>(vertex_offset_matrix);
        auto edge_smoothing_params = std::make_shared<EdgeSmoothingParams>(0.5f);

        igl::opengl::glfw::Viewer viewer;
        viewer.data().set_mesh(mesh->vertices()->cast<double>(), *mesh->faces());


        bool is_vertex_offset_last = false;
        viewer.callback_key_down = [&is_vertex_offset_last, deformation_controller, vertex_offset_params, edge_smoothing_params, mesh](igl::opengl::glfw::Viewer& viewer, unsigned char key, int) {
            if (key == ' ') {
                sptr<IDeformationParams> params;
                if (!is_vertex_offset_last) {
                    params = vertex_offset_params;
                } else {
                    params = edge_smoothing_params;
                }
                auto mesh_future = deformation_controller->applyDeformation(mesh->family_id(), params);
                then<MeshSnapshotData>(mesh_future, [&viewer](auto data) {
                    viewer.data().clear();
                    viewer.data().set_mesh(data.mesh->vertices()->template cast<double>(), *data.mesh->faces()); // Can be improved due to ordering issues.
                });
                is_vertex_offset_last = !is_vertex_offset_last;
            }
            return false;
        };

        viewer.launch();
        return 0;
}
