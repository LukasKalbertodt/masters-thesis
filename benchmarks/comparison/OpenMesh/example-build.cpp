#include <iostream>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

int main() {
    MyMesh mesh;

    auto v0 = mesh.add_vertex(MyMesh::Point(0.0, 0.0, 0.0));
    auto v1 = mesh.add_vertex(MyMesh::Point(0.0, 1.0, 0.0));
    auto v2 = mesh.add_vertex(MyMesh::Point(1.0, 0.0, 0.0));
    auto v3 = mesh.add_vertex(MyMesh::Point(1.0, 1.0, 0.0));

    mesh.add_face(v0, v2, v1);
    mesh.add_face(v2, v3, v1);

    if (!OpenMesh::IO::write_mesh(mesh, "output.ply")) {
        std::cerr << "Error writing file" << std::endl;
        return 1;
    }
}
