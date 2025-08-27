cmake_minimum_required(VERSION 3.10)
project(mesh-deformations)

set(CMAKE_CXX_STANDARD 20)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(libigl)
igl_include(glfw)

add_executable(mesh-deformations main.cpp
        include/structures/mesh.hpp
        include/utils/data_types.hpp
        src/structures/mesh.cpp
        include/utils/hashes.hpp
        include/deformation/deformation_interface.hpp
        include/deformation/deformation_params_interface.hpp
        include/deformation/deformation_task_interface.hpp
        include/deformation/deformations_snapshot.hpp
        src/deformation/deformations_snapshot.cpp
        include/cache/deformations_cache.hpp
        include/cache/deformations_cache_interface.hpp
        src/cache/deformations_cache.cpp
        include/threadpool/threadpool.hpp
        include/deformation/deformation_controller.hpp
        include/deformation/deformation_controller_interface.hpp
        src/deformation/deformation_controller.cpp
        include/utils/deformation_registry.hpp
        include/deformation/deformation_factory.hpp
)
target_include_directories(mesh-deformations PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(mesh-deformations PUBLIC igl::core igl::glfw)
