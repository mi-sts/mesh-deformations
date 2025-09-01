#pragma once

#include "igl/readOBJ.h"
#include "structures/mesh.hpp"
#include "utils/data_types.hpp"

#include <memory>
#include <string>

sptr<Mesh> loadObjMesh(std::string model_path) {
    MatrixX3f vertices;
    MatrixX3i faces;

    if (!igl::readOBJ(model_path, vertices, faces)) {
        std::cerr << "Unable to open obj model file " << model_path << std::endl;
        return nullptr;
    }

    return std::make_shared<Mesh>(vertices, faces);
}
