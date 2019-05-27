#include <OpenMesh/Core/IO/MeshIO.hh>


Outcome read_bench(const char* filename, const char* ext) {
    auto file = read_file(filename);

    return run_bench([&file, ext]() {
        MyMesh mesh;

        // Stream from memory
        auto input = std::istringstream(file);

        // Tell OpenMesh that the file is binary
        OpenMesh::IO::Options ropt;
        ropt += OpenMesh::IO::Options::Binary;

        if (!OpenMesh::IO::read_mesh(mesh, input, ext, ropt)) {
            throw "reading failed"; // should never happen
        }

        return mesh;
    });
}


#define GEN_READ_BENCH(name, mesh, ext) Outcome name() {       \
    return read_bench("../../../data/" mesh "." ext, ext);     \
}

GEN_READ_BENCH(read_mem_ply_cat, "cat", "ply")
GEN_READ_BENCH(read_mem_stl_cat, "cat", "stl")
GEN_READ_BENCH(read_mem_ply_tiger, "tiger", "ply")
GEN_READ_BENCH(read_mem_stl_tiger, "tiger", "stl")
