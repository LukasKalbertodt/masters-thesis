#include <iostream>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/Sqrt3T.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

int main() {
    MyMesh mesh;
    if (!OpenMesh::IO::read_mesh(mesh, "input.stl")) {
        std::cerr << "Error reading file" << std::endl;
        return 1;
    }

    OpenMesh::Subdivider::Uniform::Sqrt3T<MyMesh, float>()(mesh, 1);

    if (!OpenMesh::IO::write_mesh(mesh, "output.ply")) {
        std::cerr << "Error writing file" << std::endl;
        return 1;
    }
}
