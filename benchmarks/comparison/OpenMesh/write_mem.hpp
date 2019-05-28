#include <OpenMesh/Core/IO/MeshIO.hh>


Outcome write_mem_bench(const char* filename, const char* ext) {
    MyMesh mesh;

    if (!OpenMesh::IO::read_mesh(mesh, filename)) {
        throw "reading failed in write bench";
    }

    // Tell OpenMesh to write the file in binary mode
    OpenMesh::IO::Options wopt;
    wopt += OpenMesh::IO::Options::Binary;
    wopt += OpenMesh::IO::Options::LSB;

    // Write to memory
    auto writer = std::ostringstream();

    return run_bench([&mesh, &writer, ext, wopt]() {
        writer.seekp(0);

        if (!OpenMesh::IO::write_mesh(mesh, writer, ext, wopt)) {
            throw "writing failed"; // should never happen
        }

        return writer.str();
    });
}


#define GEN_WRITE_MEM_BENCH(name, mesh, ext) Outcome name() {       \
    return write_mem_bench("../../../data/" mesh ".ply", ext);     \
}

GEN_WRITE_MEM_BENCH(write_mem_ply_cat, "cat", "ply")
GEN_WRITE_MEM_BENCH(write_mem_stl_cat, "cat", "stl")
GEN_WRITE_MEM_BENCH(write_mem_ply_tiger, "tiger", "ply")
GEN_WRITE_MEM_BENCH(write_mem_stl_tiger, "tiger", "stl")