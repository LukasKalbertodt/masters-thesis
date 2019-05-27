using pmp::SurfaceMesh;


Outcome read_bench(const char* filename) {
    return run_bench([filename]() {
        SurfaceMesh mesh;
        mesh.read(filename);
        return mesh;
    });
}

#define GEN_READ_BENCH(name, mesh, ext) Outcome name() {       \
    return read_bench("/mnt/ramdisk/" mesh "." ext);     \
}

GEN_READ_BENCH(read_file_ply_cat, "cat", "ply")
GEN_READ_BENCH(read_file_stl_cat, "cat", "stl")
GEN_READ_BENCH(read_file_ply_tiger, "tiger", "ply")
GEN_READ_BENCH(read_file_stl_tiger, "tiger", "stl")
