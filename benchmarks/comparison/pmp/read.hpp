using pmp::SurfaceMesh;

// NOTE: PMP cannot read files from memory, thus this has disc access overhead.
// However, usually, operation systems cache files aggressively meaning that
// the file should already be completely in memory after the warmup.
Outcome read_bench(const char* filename) {
    return run_bench([filename]() {
        SurfaceMesh mesh;
        mesh.read(filename);
        return mesh;
    });
}

#define GEN_READ_BENCH(name, mesh, ext) Outcome name() {       \
    return read_bench("../../../data/" mesh "." ext);     \
}

GEN_READ_BENCH(read_ply_cat, "cat", "ply")
GEN_READ_BENCH(read_stl_cat, "cat", "stl")
GEN_READ_BENCH(read_ply_tiger, "tiger", "ply")
GEN_READ_BENCH(read_stl_tiger, "tiger", "stl")
