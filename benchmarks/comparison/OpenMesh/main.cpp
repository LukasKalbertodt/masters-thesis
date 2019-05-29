#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include "../util.hpp"



// Define mesh type: HEM without `prev`
struct MyTraits: public OpenMesh::DefaultTraits {
    // Do not store the previous half edge.
    HalfedgeAttributes(0);
};

using MyMesh = OpenMesh::TriMesh_ArrayKernelT<MyTraits>;


#include "algo.hpp"
#include "read_mem.hpp"
#include "read_file.hpp"
#include "write_mem.hpp"
#include "write_file.hpp"



int main(int argc, const char* argv[]) {
    try {
        run_all_benches(argc, argv, {
            // Reading
            BENCH(read_mem_ply_cat),
            BENCH(read_mem_stl_cat),
            BENCH(read_mem_ply_tiger),
            BENCH(read_mem_stl_tiger),

            BENCH(read_file_ply_cat),
            BENCH(read_file_stl_cat),
            BENCH(read_file_ply_tiger),
            BENCH(read_file_stl_tiger),

            // Writing
            BENCH(write_mem_ply_cat),
            BENCH(write_mem_stl_cat),
            BENCH(write_mem_ply_tiger),
            BENCH(write_mem_stl_tiger),

            BENCH(write_file_ply_cat),
            BENCH(write_file_stl_cat),
            BENCH(write_file_ply_tiger),
            BENCH(write_file_stl_tiger),

            // Algo
            BENCH(algo_smooth_cat),
            BENCH(algo_smooth_tiger),

            BENCH(algo_sqrt3_cat),
            BENCH(algo_sqrt3_tiger),

            BENCH(algo_count_boundary_cat),
            BENCH(algo_count_boundary_tiger),

            BENCH(algo_calc_normals_cat),
            BENCH(algo_calc_normals_tiger),
        });
    } catch (const char* e) {
        cout << endl;
        cout << "Exception was thrown: " << e << endl;
        cout << "Make sure to run the benchmark from 'comparison/OpenMesh/build'!" << endl;
        cout << "For some benchmarks, a RAM disk is required. See README.md!" << endl;
    }
}
