Mesh Deformations

This project demonstrates mesh deformation algorithms (vertex offset and edge smoothing).

------------------------------------------------------------------------

Requirements

-   CMake >= 3.10
-   C++17 compatible compiler (GCC, Clang, or MSVC)

------------------------------------------------------------------------

Build Instructions

1. Clone the repository
```
git clone https://github.com/yourname/mesh-deformations.git
cd mesh-deformations
```
2. Configure and build
```
mkdir build
cmake -B build/ -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build/
```
The main executable will be located in:
```
build/Release/mesh-deformations
```
------------------------------------------------------------------------

Build with Unit and Performance Tests

By default, unit and performance tests are disabled.
To enable them, pass the BUILD_TESTING=ON option:

This will build: 
- **mesh-deformations** — a demo application that visualizes deformations on test meshes. **Not used for benchmarking, only for demonstration purposes.**
- **mesh-deformations-tests** — unit tests for verifying deformation algorithms.
- **mesh-deformations-benchmark** — performance benchmark that measures efficiency of deformation algorithms. The Vertex Offset and Edge Smooting deformations are applied in parallel sequentially. **The benchmark can be configured via config.txt in a build folder of the mesh-deformations-benchmark**.

------------------------------------------------------------------------
### Executable paths may differ on different compilers.
------------------------------------------------------------------------

Running Unit Tests

After building with -DBUILD_TESTING=ON:
```
cd build/test/
ctest -V 
```
------------------------------------------------------------------------

Running Benchmark

After building with -DBUILD_TESTING=ON:
```
cd build/benchmark/
./mesh-deformations-benchmark.exe
```
------------------------------------------------------------------------

Notes

-   Use -DCMAKE_BUILD_TYPE=Release when running benchmarks for accurate performance results.
-   Models from the models/ directory are automatically copied next to the executable folder after build.
