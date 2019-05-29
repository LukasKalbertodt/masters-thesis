#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/Sqrt3T.hh>
#include <OpenMesh/Core/Utils/PropertyManager.hh>

// ===== Smoothing =============================================================================
Outcome smooth_bench(const char* filename) {
    MyMesh original;
    if (!OpenMesh::IO::read_mesh(original, filename)) {
        throw "reading failed"; // should never happen
    }


    return run_bench([&original]() {
        auto mesh = MyMesh(original);

        // The following code is taken from the OpenMesh tutorial and adjusted!
        // --------------------------------------------------------------------
        // this vector stores the computed centers of gravity
        auto cogs = OpenMesh::makeTemporaryProperty<OpenMesh::VertexHandle, MyMesh::Point>(mesh);

        for (const auto& vh : mesh.vertices()) {
            cogs[vh] = {0, 0, 0};
            int valence = 0;
            // Iterate over all 1-ring vertices around vh
            for (const auto& vvh : mesh.vv_range(vh)) {
                cogs[vh] += mesh.point(vvh);
                ++valence;
            }
            cogs[vh] /= valence;
        }

        // Move all vertices to the previously computed positions
        for (const auto& vh : mesh.vertices()) {
            mesh.point(vh) = cogs[vh];
        }
        // --------------------------------------------------------------------

        return mesh;
    });
}


#define GEN_SMOOTH_BENCH(name, mesh, ext) Outcome name() {       \
    return smooth_bench("../../../data/" mesh "." ext);     \
}

GEN_SMOOTH_BENCH(algo_smooth_cat, "cat", "ply")
GEN_SMOOTH_BENCH(algo_smooth_tiger, "tiger", "ply")


// ===== SQRT3 =============================================================================
Outcome sqrt3_bench(const char* filename) {
    MyMesh original;
    if (!OpenMesh::IO::read_mesh(original, filename)) {
        throw "reading failed"; // should never happen
    }

    return run_bench([&original]() {
        auto mesh = MyMesh(original);
        OpenMesh::Subdivider::Uniform::Sqrt3T<MyMesh, float>()(mesh, 1);
        return mesh;
    });
}


#define GEN_SQRT3_BENCH(name, mesh, ext) Outcome name() {       \
    return sqrt3_bench("../../../data/" mesh "." ext);     \
}

GEN_SQRT3_BENCH(algo_sqrt3_cat, "cat", "ply")
GEN_SQRT3_BENCH(algo_sqrt3_tiger, "tiger", "ply")


// ===== Boundary Count ========================================================================
Outcome boundary_count_bench(const char* filename) {
    MyMesh original;
    if (!OpenMesh::IO::read_mesh(original, filename)) {
        throw "reading failed"; // should never happen
    }

    const auto& mesh = original;
    return run_bench([&mesh]() {
        size_t count = 0;
        for (const auto& vh: mesh.vertices()) {
            if (!mesh.is_boundary(vh)) {
                count += 1;
            }
        }

        return count;
    });
}


#define GEN_BOUNDARY_COUNT_BENCH(name, mesh, ext) Outcome name() {       \
    return boundary_count_bench("../../../data/" mesh "." ext);     \
}

GEN_BOUNDARY_COUNT_BENCH(algo_count_boundary_cat, "cat", "ply")
GEN_BOUNDARY_COUNT_BENCH(algo_count_boundary_tiger, "tiger", "ply")


// ===== Normals ============================================================================
Outcome calc_normals_bench(const char* filename) {
    MyMesh original;
    if (!OpenMesh::IO::read_mesh(original, filename)) {
        throw "reading failed"; // should never happen
    }

    const auto& mesh = original;
    return run_bench([&mesh]() {
        std::vector<MyMesh::Point> normals;
        normals.reserve(mesh.n_faces());

        for (const auto& fh: mesh.faces()) {
            normals.push_back(mesh.calc_face_normal(fh));
        }

        return normals;
    });
}


#define GEN_CALC_NORMALS_BENCH(name, mesh, ext) Outcome name() {       \
    return calc_normals_bench("../../../data/" mesh "." ext);     \
}

GEN_CALC_NORMALS_BENCH(algo_calc_normals_cat, "cat", "ply")
GEN_CALC_NORMALS_BENCH(algo_calc_normals_tiger, "tiger", "ply")
