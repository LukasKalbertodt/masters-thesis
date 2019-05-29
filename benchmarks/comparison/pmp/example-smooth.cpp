#include <pmp/SurfaceMesh.h>

using pmp::SurfaceMesh;
using pmp::Point;
using pmp::vec3;

int main() {
    SurfaceMesh mesh;
    if (!mesh.read("input.stl")) {
        std::cerr << "Error reading file" << std::endl;
        return 1;
    }

    auto new_pos = mesh.add_vertex_property<Point>("v:new_pos");
    auto points = mesh.get_vertex_property<Point>("v:point");

    for (const auto v: mesh.vertices()) {
        if (mesh.is_boundary(v)) {
            new_pos[v] = points[v];
        } else {
            auto total_displacement = vec3(0, 0, 0);
            size_t valence = 0;

            for (const auto neighbor: mesh.vertices(v)) {
                total_displacement += points[neighbor];
                valence += 1;
            }

            new_pos[v] = total_displacement / valence;
        }
    }

    for (const auto v: mesh.vertices()) {
        points[v] = new_pos[v];
    }

    mesh.remove_vertex_property(new_pos);


    if (!mesh.write("output.ply")) {
        std::cerr << "Error writing file" << std::endl;
        return 1;
    }
}
