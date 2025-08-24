#include <igl/opengl/glfw/Viewer.h>
#include <Eigen/Core>

int main() {
    Eigen::MatrixXd V(4,3);
    Eigen::MatrixXi F(2,3);

    V << 0,0,0, 1,0,0, 1,1,0, 0,1,0;
    F << 0,1,2, 0,2,3;

    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(V,F);
    viewer.launch();
}
