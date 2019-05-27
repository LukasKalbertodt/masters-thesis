#include <pmp/SurfaceMesh.h>
#include "../util.hpp"

#include "read_file.hpp"
#include "write_file.hpp"


int main(int argc, const char *argv[]) {
    run_all_benches(argc, argv, {
        BENCH(read_file_ply_cat),
        BENCH(read_file_stl_cat),
        BENCH(read_file_ply_tiger),
        BENCH(read_file_stl_tiger),

        BENCH(write_file_ply_cat),
        BENCH(write_file_stl_cat),
        BENCH(write_file_ply_tiger),
        BENCH(write_file_stl_tiger),
    });
}
