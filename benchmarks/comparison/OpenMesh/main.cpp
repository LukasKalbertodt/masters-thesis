#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include "../util.hpp"



// Define mesh type: HEM without `prev`
struct MyTraits: public OpenMesh::DefaultTraits {
    // Do not store the previous half edge.
    HalfedgeAttributes(0);
};

using MyMesh = OpenMesh::TriMesh_ArrayKernelT<MyTraits>;


#include "read_mem.hpp"
#include "read_file.hpp"
#include "write_mem.hpp"
#include "write_file.hpp"



int main(int argc, const char* argv[]) {
    run_all_benches(argc, argv, {
        BENCH(read_mem_ply_cat),
        BENCH(read_mem_stl_cat),
        BENCH(read_mem_ply_tiger),
        BENCH(read_mem_stl_tiger),

        BENCH(read_file_ply_cat),
        BENCH(read_file_stl_cat),
        BENCH(read_file_ply_tiger),
        BENCH(read_file_stl_tiger),

        BENCH(write_mem_ply_cat),
        BENCH(write_mem_stl_cat),
        BENCH(write_mem_ply_tiger),
        BENCH(write_mem_stl_tiger),

        BENCH(write_file_ply_cat),
        BENCH(write_file_stl_cat),
        BENCH(write_file_ply_tiger),
        BENCH(write_file_stl_tiger),
    });
}
