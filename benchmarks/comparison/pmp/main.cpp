#include <pmp/SurfaceMesh.h>
#include "../util.hpp"

#include "algo.hpp"
#include "read_file.hpp"
#include "write_file.hpp"


int main(int argc, const char *argv[]) {
    run_all_benches(argc, argv, {
        // Read
        BENCH(read_file_ply_cat),
        BENCH(read_file_stl_cat),
        BENCH(read_file_ply_tiger),
        BENCH(read_file_stl_tiger),

        // Write
        BENCH(write_file_ply_cat),
        BENCH(write_file_stl_cat),
        BENCH(write_file_ply_tiger),
        BENCH(write_file_stl_tiger),

        // Algorithms
        BENCH(algo_smooth_cat),
        BENCH(algo_smooth_tiger),

        BENCH(algo_sqrt3_cat),
        BENCH(algo_sqrt3_tiger),

        BENCH(algo_boundary_vcount_cat),
        BENCH(algo_boundary_vcount_tiger),

        BENCH(algo_boundary_fcount_cat),
        BENCH(algo_boundary_fcount_tiger),

        BENCH(algo_calc_fnormals_cat),
        BENCH(algo_calc_fnormals_tiger),

        BENCH(algo_calc_vnormals_cat),
        BENCH(algo_calc_vnormals_tiger),
    });
}
