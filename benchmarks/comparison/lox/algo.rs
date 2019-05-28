use test::black_box;

use failure::Error;
use libtest_mimic::Outcome;
use lox::{
    prelude::*,
    algo,
    cgmath::prelude::*,
    ds::{
        DirectedEdgeMesh, HalfEdgeMesh, SharedVertexMesh,
        half_edge::TriConfig,
    },
    fat::MiniMesh,
    map::VecMap,
    io,
};

use crate::util::do_bench;


// ===== Smoothing =============================================================================
macro_rules! gen_smooth {
    ($fn_name:ident, $file:literal, $mesh:ty) => {
        pub fn $fn_name() -> Result<Outcome, Error> {
            let mesh: MiniMesh<$mesh> = io::read_file(&format!("data/{}", $file))?;

            let out = do_bench(|| {
                let mut clone = mesh.clone();
                let new_pos = algo::smooth_simple(&clone.mesh, &clone.vertex_positions);
                clone.vertex_positions = new_pos;
                clone
            });

            Ok(out)
        }
    }
}

gen_smooth!(smooth_cat_hem, "cat.ply", HalfEdgeMesh<TriConfig>);
gen_smooth!(smooth_cat_dem, "cat.ply", DirectedEdgeMesh);
gen_smooth!(smooth_tiger_hem, "tiger.ply", HalfEdgeMesh<TriConfig>);
gen_smooth!(smooth_tiger_dem, "tiger.ply", DirectedEdgeMesh);


// ===== SQRT3 =============================================================================
macro_rules! gen_sqrt3 {
    ($fn_name:ident, $file:literal) => {
        pub fn $fn_name() -> Result<Outcome, Error> {
            let mesh: MiniMesh<HalfEdgeMesh<TriConfig>>
                = io::read_file(&format!("data/{}", $file))?;

            let out = do_bench(|| {
                let mut clone = mesh.clone();
                algo::subdivision::sqrt3(
                    &mut clone.mesh,
                    &mut clone.vertex_positions,
                    1,
                );
                clone
            });

            Ok(out)
        }
    }
}

gen_sqrt3!(sqrt3_cat_hem, "cat.ply");
gen_sqrt3!(sqrt3_tiger_hem, "tiger.ply");



// ===== Boundary Count ========================================================================
macro_rules! gen_boundary_count {
    ($fn_name:ident, $file:literal, $mesh:ty) => {
        pub fn $fn_name() -> Result<Outcome, Error> {
            let mesh: MiniMesh<$mesh> = io::read_file(&format!("data/{}", $file))?;

            let out = do_bench(|| {
                black_box(&mesh.mesh)
                    .vertices()
                    .filter(|v| v.is_boundary())
                    .count()
            });

            Ok(out)
        }
    }
}

gen_boundary_count!(boundary_count_cat_hem, "cat.ply", HalfEdgeMesh<TriConfig>);
gen_boundary_count!(boundary_count_cat_dem, "cat.ply", DirectedEdgeMesh);
gen_boundary_count!(boundary_count_tiger_hem, "tiger.ply", HalfEdgeMesh<TriConfig>);
gen_boundary_count!(boundary_count_tiger_dem, "tiger.ply", DirectedEdgeMesh);



// ===== Normals ============================================================================
macro_rules! gen_calc_normals {
    ($fn_name:ident, $file:literal, $mesh:ty) => {
        pub fn $fn_name() -> Result<Outcome, Error> {
            let mesh: MiniMesh<$mesh> = io::read_file(&format!("data/{}", $file))?;

            let out = do_bench(|| {
                let mesh = black_box(&mesh);
                mesh.mesh
                    .face_handles()
                    .map(|fh| {
                        let [a, b, c] = mesh.mesh.vertices_around_triangle(fh);
                        let [pa, pb, pc] = [
                            mesh.vertex_positions[a],
                            mesh.vertex_positions[b],
                            mesh.vertex_positions[c],
                        ];
                        let normal = (pb - pa).cross(pc - pa).normalize();

                        (fh, normal)
                    })
                    .collect::<VecMap<_, _>>()
            });

            Ok(out)
        }
    }
}

gen_calc_normals!(calc_normals_cat_hem, "cat.ply", HalfEdgeMesh<TriConfig>);
gen_calc_normals!(calc_normals_cat_dem, "cat.ply", DirectedEdgeMesh);
gen_calc_normals!(calc_normals_cat_svm, "cat.ply", SharedVertexMesh);
gen_calc_normals!(calc_normals_tiger_hem, "tiger.ply", HalfEdgeMesh<TriConfig>);
gen_calc_normals!(calc_normals_tiger_dem, "tiger.ply", DirectedEdgeMesh);
gen_calc_normals!(calc_normals_tiger_svm, "tiger.ply", SharedVertexMesh);
