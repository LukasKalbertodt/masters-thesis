#include <OpenMesh/Core/IO/MeshIO.hh>


Outcome write_file_bench(const char* infile, const char* outfile) {
    MyMesh mesh;

    if (!OpenMesh::IO::read_mesh(mesh, infile)) {
        throw "reading failed in write bench";
    }

    return run_bench([&mesh, outfile]() {
        if (!OpenMesh::IO::write_mesh(mesh, outfile)) {
            throw "writing failed"; // should never happen
        }

        return "dummy";
    });
}


#define GEN_WRITE_FILE_BENCH(name, mesh, ext) Outcome name() {       \
    return write_file_bench("/mnt/ramdisk/" mesh ".ply", "/mnt/ramdisk/_out_" mesh ".ply");     \
}

GEN_WRITE_FILE_BENCH(write_file_ply_cat, "cat", "ply")
GEN_WRITE_FILE_BENCH(write_file_stl_cat, "cat", "stl")
GEN_WRITE_FILE_BENCH(write_file_ply_tiger, "tiger", "ply")
GEN_WRITE_FILE_BENCH(write_file_stl_tiger, "tiger", "stl")
