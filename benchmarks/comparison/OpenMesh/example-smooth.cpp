#include <iostream>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Utils/PropertyManager.hh>

typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

int main() {
    MyMesh mesh;
    if (!OpenMesh::IO::read_mesh(mesh, "input.stl")) {
        std::cerr << "Error reading file" << std::endl;
        return 1;
    }

    auto cogs = OpenMesh::makeTemporaryProperty<OpenMesh::VertexHandle, MyMesh::Point>(mesh);

    for (const auto& vh : mesh.vertices()) {
        if (mesh.is_boundary(vh)) {
            cogs[vh] = mesh.point(vh);
        } else {
            cogs[vh] = {0, 0, 0};
            int valence = 0;

            for (const auto& vvh : mesh.vv_range(vh)) {
                cogs[vh] += mesh.point(vvh);
                ++valence;
            }
            cogs[vh] /= valence;
        }
    }

    for (const auto& vh : mesh.vertices()) {
        mesh.point(vh) = cogs[vh];
    }

    if (!OpenMesh::IO::write_mesh(mesh, "output.ply")) {
        std::cerr << "Error writing file" << std::endl;
        return 1;
    }
}
