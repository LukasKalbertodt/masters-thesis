#include <OpenMesh/Core/IO/MeshIO.hh>


Outcome smooth_bench(const char* filename) {
    MyMesh original;
    if (!OpenMesh::IO::read_mesh(original, filename)) {
        throw "reading failed"; // should never happen
    }


    return run_bench([&original]() {
        auto mesh = original;

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
