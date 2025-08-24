cmake_minimum_required(VERSION 3.10)
project(mesh-deformations)

set(CMAKE_CXX_STANDARD 20)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(libigl)
igl_include(glfw)

add_executable(mesh-deformations main.cpp)
target_link_libraries(mesh-deformations PUBLIC igl::core igl::glfw)

