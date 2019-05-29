#include <pmp/algorithms/SurfaceNormals.h>

using pmp::SurfaceMesh;
using pmp::Normal;
using pmp::Point;
using pmp::IOFlags;


Outcome write_bench(const char* infile, const char* outfile, const char* ext) {
    SurfaceMesh mesh;
    mesh.read(infile);

    // Generate face normals if we are writing STL
    if (string(ext) == "stl") {
        pmp::SurfaceNormals::compute_face_normals(mesh);
    }

    auto flags = IOFlags();
    flags.use_binary = true;

    return run_bench([&mesh, outfile, flags]() {
        mesh.write(outfile, flags);
        return "dummy";
    });
}

#define GEN_WRITE_BENCH(name, mesh, ext) Outcome name() {       \
    return write_bench("/mnt/ramdisk/" mesh "." ext, "/mnt/ramdisk/_out_" mesh "." ext, ext);     \
}

GEN_WRITE_BENCH(write_file_ply_cat, "cat", "ply")
GEN_WRITE_BENCH(write_file_stl_cat, "cat", "stl")
GEN_WRITE_BENCH(write_file_ply_tiger, "tiger", "ply")
GEN_WRITE_BENCH(write_file_stl_tiger, "tiger", "stl")
