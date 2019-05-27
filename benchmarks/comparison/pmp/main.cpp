#include <pmp/SurfaceMesh.h>
#include "../util.hpp"

#include "read.hpp"


int main(int argc, const char *argv[]) {
    run_all_benches(argc, argv, {
        BENCH(read_ply_cat),
        BENCH(read_stl_cat),
        BENCH(read_ply_tiger),
        BENCH(read_stl_tiger),

        // BENCH(write_ply_cat),
        // BENCH(write_stl_cat),
        // BENCH(write_ply_tiger),
        // BENCH(write_stl_tiger),
    });
}
