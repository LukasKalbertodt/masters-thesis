#include <pmp/SurfaceMesh.h>

using pmp::SurfaceMesh;

int main() {
    SurfaceMesh mesh;
    if (!mesh.read("input.stl")) {
        std::cerr << "Error reading file" << std::endl;
        return 1;
    }

    if (!mesh.write("output.ply")) {
        std::cerr << "Error writing file" << std::endl;
        return 1;
    }
}
