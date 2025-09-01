cmake_minimum_required(VERSION 3.10)

include(FetchContent)

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.10.0
)
FetchContent_MakeAvailable(catch2)

file(GLOB TEST_SOURCES "*.cpp")

add_executable(mesh_deformations_tests ${TEST_SOURCES})

target_link_libraries(mesh_deformations_tests PRIVATE mesh_lib Catch2::Catch2WithMain)

include(CTest)
include(Catch)

catch_discover_tests(mesh_deformations_tests)
