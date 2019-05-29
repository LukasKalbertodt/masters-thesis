#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/Sqrt3T.hh>

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
        std::vector<MyMesh::Point>  cogs;
        std::vector<MyMesh::Point>::iterator cog_it;
        cogs.reserve(mesh.n_vertices());

        // smoothing mesh argv[1] times
        MyMesh::VertexIter          v_it, v_end(mesh.vertices_end());
        MyMesh::VertexVertexIter    vv_it;
        MyMesh::Point               cog;
        MyMesh::Scalar              valence;

        for (v_it=mesh.vertices_begin(); v_it!=v_end; ++v_it) {
            cog[0] = cog[1] = cog[2] = valence = 0.0;

            for (vv_it=mesh.vv_iter( *v_it ); vv_it.is_valid(); ++vv_it) {
                cog += mesh.point( *vv_it );
                ++valence;
            }
            cogs.push_back(cog / valence);
        }

        for (v_it=mesh.vertices_begin(), cog_it=cogs.begin();
             v_it!=v_end; ++v_it, ++cog_it) {
            if ( !mesh.is_boundary( *v_it ) ) {
                mesh.set_point( *v_it, *cog_it );
            }
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
        auto v_end = mesh.vertices_end();
        for (auto v_it = mesh.vertices_begin(); v_it != v_end; ++v_it) {
            if (!mesh.is_boundary( *v_it )) {
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

        auto f_end = mesh.faces_end();
        for (auto f_it = mesh.faces_begin(); f_it != f_end; ++f_it) {
            normals.push_back(mesh.calc_face_normal(*f_it));
        }

        return normals;
    });
}


#define GEN_CALC_NORMALS_BENCH(name, mesh, ext) Outcome name() {       \
    return calc_normals_bench("../../../data/" mesh "." ext);     \
}

GEN_CALC_NORMALS_BENCH(algo_calc_normals_cat, "cat", "ply")
GEN_CALC_NORMALS_BENCH(algo_calc_normals_tiger, "tiger", "ply")
