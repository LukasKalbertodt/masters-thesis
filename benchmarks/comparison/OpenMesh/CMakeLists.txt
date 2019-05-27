project(openmesh-benchmark)
cmake_minimum_required(VERSION 3.10.2)

# We are compiling a benchmark, after all
set(CMAKE_BUILD_TYPE Release)

# Compile OpenMesh
add_subdirectory(ext/OpenMesh/)

# Add OpenMesh include directories
include_directories(ext/OpenMesh/src)

# Add benchmark executable
set(name om-bench)
add_executable (${name} main.cpp)
target_link_libraries (${name}
  OpenMeshCore
)
