project(pmp-benchmark)
cmake_minimum_required(VERSION 3.10.2)

# We are compiling a benchmark, after all
set(CMAKE_BUILD_TYPE Release)


# Compile PMP
set(PMP_BUILD_APPS     OFF CACHE BOOL "")
set(PMP_BUILD_EXAMPLES OFF CACHE BOOL "")
set(PMP_BUILD_TESTS    OFF CACHE BOOL "")
set(PMP_BUILD_DOCS     OFF CACHE BOOL "")
add_subdirectory(ext/pmp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra")

include_directories(${PROJECT_SOURCE_DIR}/ext/pmp/src)


# Add benchmark executable
set(name pmp-bench)
add_executable (${name} main.cpp)
target_link_libraries (${name}
  pmp
)

add_executable (example-io example-io.cpp)
target_link_libraries (example-io pmp)
