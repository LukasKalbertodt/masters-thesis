#include <OpenMesh/Core/IO/MeshIO.hh>


Outcome read_file_bench(const char* filename) {
    return run_bench([filename]() {
        MyMesh mesh;

        if (!OpenMesh::IO::read_mesh(mesh, filename)) {
            throw "reading failed"; // should never happen
        }

        return mesh;
    });
}


#define GEN_READ_FILE_BENCH(name, mesh, ext) Outcome name() {       \
    return read_file_bench("/mnt/ramdisk/" mesh "." ext);     \
}

GEN_READ_FILE_BENCH(read_file_ply_cat, "cat", "ply")
GEN_READ_FILE_BENCH(read_file_stl_cat, "cat", "stl")
GEN_READ_FILE_BENCH(read_file_ply_tiger, "tiger", "ply")
GEN_READ_FILE_BENCH(read_file_stl_tiger, "tiger", "stl")
