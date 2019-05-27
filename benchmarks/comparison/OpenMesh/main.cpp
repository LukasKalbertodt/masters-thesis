#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include "../util.hpp"



// Define mesh type: HEM without `prev`
struct MyTraits: public OpenMesh::DefaultTraits {
    // Do not store the previous half edge.
    HalfedgeAttributes(0);
};

using MyMesh = OpenMesh::TriMesh_ArrayKernelT<MyTraits>;


#include "read.hpp"



int main(int argc, const char* argv[]) {
    run_all_benches(argc, argv, {
        BENCH(read_ply_cat),
        BENCH(read_stl_cat),
        BENCH(read_ply_tiger),
        BENCH(read_stl_tiger),
    });
}
