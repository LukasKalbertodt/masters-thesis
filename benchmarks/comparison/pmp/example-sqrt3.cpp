#include <pmp/algorithms/SurfaceSubdivision.h>
#include <pmp/SurfaceMesh.h>

using pmp::SurfaceMesh;
using pmp::SurfaceSubdivision;

int main() {
    SurfaceMesh mesh;
    if (!mesh.read("input.stl")) {
        std::cerr << "Error reading file" << std::endl;
        return 1;
    }

    auto sub = SurfaceSubdivision(mesh);
    sub.sqrt3();

    if (!mesh.write("output.ply")) {
        std::cerr << "Error writing file" << std::endl;
        return 1;
    }
}
