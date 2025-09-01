cmake_minimum_required(VERSION 3.10)
project(mesh-deformations)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTING "Build unit and performance tests" OFF)

set(MODELS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/models)
set(MODELS_TARGET_DIR $<TARGET_FILE_DIR:mesh-deformations>/models)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(libigl)
igl_include(glfw)
include(xxhash)

add_library(mesh-deformations-lib  STATIC
        include/structures/mesh.hpp
        src/structures/mesh.cpp
        include/utils/data_types.hpp
        include/utils/hashes.hpp
        include/deformation/deformation_interface.hpp
        include/deformation/deformation_params_interface.hpp
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
        include/deformation/vertex_offset_deformation.hpp
        include/deformation/edge_smoothing_deformation.hpp
        include/utils/filesystem.hpp
        include/utils/containers.hpp
        include/utils/mutithreading.hpp
        include/utils/mesh_loading.hpp
)

target_include_directories(mesh-deformations-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(mesh-deformations-lib PUBLIC igl::core igl::glfw xxhash)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Configuring Release build")
    target_compile_definitions(mesh-deformations-lib PUBLIC NDEBUG)
    target_compile_options(mesh-deformations-lib PRIVATE -O3 -march=native)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Configuring Debug build")
    target_compile_definitions(mesh-deformations-lib PUBLIC DEBUG)
    target_compile_options(mesh-deformations-lib PRIVATE -g)
endif()


add_executable(mesh-deformations main.cpp)
target_link_libraries(mesh-deformations PRIVATE mesh-deformations-lib)

file(GLOB MODELS_FILES "${MODELS_DIR}/*.obj")

foreach(model ${MODELS_FILES})
    add_custom_command(
            TARGET mesh-deformations POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${MODELS_TARGET_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${model}
            ${MODELS_TARGET_DIR}/
    )
endforeach()

if(BUILD_TESTING)
    add_subdirectory(test)
    add_subdirectory(benchmark)
endif()
