#include <pmp/SurfaceMesh.h>

using pmp::SurfaceMesh;
using pmp::Point;

int main() {
    SurfaceMesh mesh;

    const auto v0 = mesh.add_vertex(Point(0.0, 0.0, 0.0));
    const auto v1 = mesh.add_vertex(Point(0.0, 1.0, 0.0));
    const auto v2 = mesh.add_vertex(Point(1.0, 0.0, 0.0));
    const auto v3 = mesh.add_vertex(Point(1.0, 1.0, 0.0));

    mesh.add_triangle(v0, v2, v1);
    mesh.add_triangle(v2, v3, v1);

    if (!mesh.write("output.ply")) {
        std::cerr << "Error writing file" << std::endl;
        return 1;
    }
}
