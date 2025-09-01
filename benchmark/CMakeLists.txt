cmake_minimum_required(VERSION 3.10)

set(BENCHMARK_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BENCHMARK_TARGET_DIR $<TARGET_FILE_DIR:mesh-deformations-benchmark>)
set(BENCHMARK_CONFIG_FILE ${BENCHMARK_DIR}/config.txt)

set(MODELS_BENCHMARK_TARGET_DIR $<TARGET_FILE_DIR:mesh-deformations-benchmark>/models)

add_executable(mesh-deformations-benchmark main.cpp
        benchmark_controller.hpp
        benchmark_config_data.hpp
        benchmark_config_parser.hpp
)

target_link_libraries(mesh-deformations-benchmark PRIVATE mesh-deformations-lib)

add_custom_command(
        TARGET mesh-deformations-benchmark POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${MODELS_TARGET_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${BENCHMARK_CONFIG_FILE}
        ${BENCHMARK_TARGET_DIR}
)

foreach(model ${MODELS_FILES})
    add_custom_command(
            TARGET mesh-deformations-benchmark POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${MODELS_BENCHMARK_TARGET_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${model}
            ${MODELS_BENCHMARK_TARGET_DIR}/
    )
endforeach()
