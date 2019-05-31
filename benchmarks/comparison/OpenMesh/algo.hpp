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


// ===== Boundary Count Vertices ================================================================
Outcome boundary_vcount_bench(const char* filename) {
    MyMesh original;
    if (!OpenMesh::IO::read_mesh(original, filename)) {
        throw "reading failed"; // should never happen
    }

    const auto& mesh = original;
    return run_bench([&mesh]() {
        size_t count = 0;
        for (const auto& vh: mesh.vertices()) {
            if (mesh.is_boundary(vh)) {
                count += 1;
            }
        }

        return count;
    });
}


#define GEN_BOUNDARY_VCOUNT_BENCH(name, mesh, ext) Outcome name() {       \
    return boundary_vcount_bench("../../../data/" mesh "." ext);     \
}

GEN_BOUNDARY_VCOUNT_BENCH(algo_vcount_boundary_cat, "cat", "ply")
GEN_BOUNDARY_VCOUNT_BENCH(algo_vcount_boundary_tiger, "tiger", "ply")



// ===== Boundary Count Faces ===================================================================
Outcome boundary_fcount_bench(const char* filename) {
    MyMesh original;
    if (!OpenMesh::IO::read_mesh(original, filename)) {
        throw "reading failed"; // should never happen
    }

    const auto& mesh = original;
    return run_bench([&mesh]() {
        size_t count = 0;
        for (const auto& vh: mesh.faces()) {
            if (mesh.is_boundary(vh)) {
                count += 1;
            }
        }

        return count;
    });
}


#define GEN_BOUNDARY_FCOUNT_BENCH(name, mesh, ext) Outcome name() {       \
    return boundary_fcount_bench("../../../data/" mesh "." ext);     \
}

GEN_BOUNDARY_FCOUNT_BENCH(algo_fcount_boundary_cat, "cat", "ply")
GEN_BOUNDARY_FCOUNT_BENCH(algo_fcount_boundary_tiger, "tiger", "ply")


// ===== Normals ============================================================================
Outcome calc_fnormals_bench(const char* filename) {
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


#define GEN_CALC_FNORMALS_BENCH(name, mesh, ext) Outcome name() {       \
    return calc_fnormals_bench("../../../data/" mesh "." ext);     \
}

GEN_CALC_FNORMALS_BENCH(algo_calc_fnormals_cat, "cat", "ply")
GEN_CALC_FNORMALS_BENCH(algo_calc_fnormals_tiger, "tiger", "ply")


// ===== Normals ============================================================================
Outcome calc_vnormals_bench(const char* filename) {
    MyMesh original;
    if (!OpenMesh::IO::read_mesh(original, filename)) {
        throw "reading failed"; // should never happen
    }

    original.request_face_normals();
    original.update_normals();

    const auto& mesh = original;
    return run_bench([&mesh]() {
        std::vector<MyMesh::Point> normals;
        normals.reserve(mesh.n_vertices());

        for (const auto& vh: mesh.vertices()) {
            auto sum = MyMesh::Point(0, 0, 0);
            for (const auto& fh: mesh.vf_range(vh)) {
                sum += mesh.normal(fh);
            }
            normals.push_back(sum.normalize());
        }

        return normals;
    });
}


#define GEN_CALC_VNORMALS_BENCH(name, mesh, ext) Outcome name() {       \
    return calc_vnormals_bench("../../../data/" mesh "." ext);     \
}

GEN_CALC_VNORMALS_BENCH(algo_calc_vnormals_cat, "cat", "ply")
GEN_CALC_VNORMALS_BENCH(algo_calc_vnormals_tiger, "tiger", "ply")
