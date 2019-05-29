#include <pmp/algorithms/SurfaceSubdivision.h>
#include <pmp/MatVec.h>
#include <pmp/algorithms/SurfaceNormals.h>

using pmp::SurfaceMesh;
using pmp::SurfaceSubdivision;
using pmp::Normal;
using pmp::Point;
using pmp::IOFlags;
using pmp::vec3;



// ===== Smoothing =============================================================================
Outcome smooth_bench(const char* file) {
    SurfaceMesh original;
    original.read(file);


    return run_bench([&original]() {
        auto mesh = SurfaceMesh(original);

        auto new_pos = mesh.add_vertex_property<Point>("v:new_pos");
        auto points = mesh.get_vertex_property<Point>("v:point");

        for (auto v: mesh.vertices()) {
            if (mesh.is_boundary(v)) {
                new_pos[v] = points[v];
            } else {
                auto total_displacement = vec3(0, 0, 0);
                size_t valence = 0;

                for (auto neighbor: mesh.vertices(v)) {
                    total_displacement += points[neighbor];
                    valence += 1;
                }

                new_pos[v] = total_displacement / valence;
            }
        }

        for (auto v: mesh.vertices()) {
            points[v] = new_pos[v];
        }

        mesh.remove_vertex_property(new_pos);

        return mesh;
    });
}


#define GEN_SMOOTH_BENCH(name, mesh) Outcome name() {       \
    return smooth_bench("../../../data/" mesh);     \
}

GEN_SMOOTH_BENCH(algo_smooth_cat, "cat.ply")
GEN_SMOOTH_BENCH(algo_smooth_tiger, "tiger.ply")


// ===== SQRT3 =============================================================================
Outcome sqrt3_bench(const char* file) {
    SurfaceMesh original;
    original.read(file);

    return run_bench([&original]() {
        auto mesh = SurfaceMesh(original);
        auto sub = SurfaceSubdivision(mesh);
        sub.sqrt3();

        return mesh;
    });
}

#define GEN_SQRT3_BENCH(name, mesh) Outcome name() {       \
    return sqrt3_bench("../../../data/" mesh);     \
}

GEN_SQRT3_BENCH(algo_sqrt3_cat, "cat.ply")
GEN_SQRT3_BENCH(algo_sqrt3_tiger, "tiger.ply")



// ===== Boundary Count ========================================================================
Outcome boundary_count_bench(const char* file) {
    SurfaceMesh original;
    original.read(file);

    const auto& mesh = original;
    return run_bench([&mesh]() {
        size_t count = 0;
        for (auto v: mesh.vertices()) {
            if (mesh.is_boundary(v)) {
                count += 1;
            }
        }

        return count;
    });
}

#define GEN_BOUNDARY_COUNT_BENCH(name, mesh) Outcome name() {       \
    return boundary_count_bench("../../../data/" mesh);     \
}

GEN_BOUNDARY_COUNT_BENCH(algo_boundary_count_cat, "cat.ply")
GEN_BOUNDARY_COUNT_BENCH(algo_boundary_count_tiger, "tiger.ply")




// ===== Normals ============================================================================
Outcome calc_normals_bench(const char* file) {
    SurfaceMesh original;
    original.read(file);

    const auto& mesh = original;
    return run_bench([&mesh]() {
        std::vector<Normal> normals;
        normals.reserve(mesh.faces_size());

        for (auto f: mesh.faces()) {
            normals.push_back(pmp::SurfaceNormals::compute_face_normal(mesh, f));
        }

        return normals;
    });
}

#define GEN_CALC_NORMALS_BENCH(name, mesh) Outcome name() {       \
    return calc_normals_bench("../../../data/" mesh);     \
}

GEN_CALC_NORMALS_BENCH(algo_calc_normals_cat, "cat.ply")
GEN_CALC_NORMALS_BENCH(algo_calc_normals_tiger, "tiger.ply")
