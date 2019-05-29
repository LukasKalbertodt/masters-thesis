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

add_executable (example-smooth example-smooth.cpp)
target_link_libraries (example-smooth OpenMeshCore)

add_executable (example-build example-build.cpp)
target_link_libraries (example-build OpenMeshCore)

add_executable (example-sqrt3 example-sqrt3.cpp)
target_link_libraries (example-sqrt3 OpenMeshCore)
