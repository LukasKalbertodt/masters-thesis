project(openmesh-benchmark)
cmake_minimum_required(VERSION 3.10.2)

# We are compiling a benchmark, after all
set(CMAKE_BUILD_TYPE Release)

# Compile OpenMesh
add_subdirectory(ext/OpenMesh/)

# Add OpenMesh include directories
include_directories(ext/OpenMesh/src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra")

# Add benchmark executable
set(name om-bench)
add_executable (${name} main.cpp)
target_link_libraries (${name}
  OpenMeshCore
)

add_executable (example-io example-io.cpp)
target_link_libraries (example-io OpenMeshCore)
