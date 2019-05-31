use test::black_box;

use failure::Error;
use libtest_mimic::Outcome;
use lox::{
    prelude::*,
    algo,
    cgmath::{Vector3, prelude::*},
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



// ===== Boundary Count Vertices =================================================================
macro_rules! gen_boundary_vcount {
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

gen_boundary_vcount!(boundary_vcount_cat_hem, "cat.ply", HalfEdgeMesh<TriConfig>);
gen_boundary_vcount!(boundary_vcount_cat_dem, "cat.ply", DirectedEdgeMesh);
gen_boundary_vcount!(boundary_vcount_tiger_hem, "tiger.ply", HalfEdgeMesh<TriConfig>);
gen_boundary_vcount!(boundary_vcount_tiger_dem, "tiger.ply", DirectedEdgeMesh);


// ===== Boundary Count Faces ===================================================================
macro_rules! gen_boundary_fcount {
    ($fn_name:ident, $file:literal, $mesh:ty) => {
        pub fn $fn_name() -> Result<Outcome, Error> {
            let mesh: MiniMesh<$mesh> = io::read_file(&format!("data/{}", $file))?;

            let out = do_bench(|| {
                black_box(&mesh.mesh)
                    .faces()
                    .filter(|f| f.is_boundary())
                    .count()
            });

            Ok(out)
        }
    }
}

gen_boundary_fcount!(boundary_fcount_cat_hem, "cat.ply", HalfEdgeMesh<TriConfig>);
gen_boundary_fcount!(boundary_fcount_cat_dem, "cat.ply", DirectedEdgeMesh);
gen_boundary_fcount!(boundary_fcount_tiger_hem, "tiger.ply", HalfEdgeMesh<TriConfig>);
gen_boundary_fcount!(boundary_fcount_tiger_dem, "tiger.ply", DirectedEdgeMesh);



// ===== Face Normals ==========================================================================
macro_rules! gen_calc_face_normals {
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

gen_calc_face_normals!(calc_fnormals_cat_hem, "cat.ply", HalfEdgeMesh<TriConfig>);
gen_calc_face_normals!(calc_fnormals_cat_dem, "cat.ply", DirectedEdgeMesh);
gen_calc_face_normals!(calc_fnormals_cat_svm, "cat.ply", SharedVertexMesh);
gen_calc_face_normals!(calc_fnormals_tiger_hem, "tiger.ply", HalfEdgeMesh<TriConfig>);
gen_calc_face_normals!(calc_fnormals_tiger_dem, "tiger.ply", DirectedEdgeMesh);
gen_calc_face_normals!(calc_fnormals_tiger_svm, "tiger.ply", SharedVertexMesh);



// ===== Vertex Normals =========================================================================
macro_rules! gen_calc_vertex_normals {
    ($fn_name:ident, $file:literal, $mesh:ty) => {
        pub fn $fn_name() -> Result<Outcome, Error> {
            let mesh: MiniMesh<$mesh> = io::read_file(&format!("data/{}", $file))?;
            let face_normals = mesh.mesh
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
                .collect::<VecMap<_, _>>();

            let out = do_bench(|| {
                let mesh = black_box(&mesh);
                mesh.mesh.vertices().map(|v| {
                    let normal = v.adjacent_faces()
                        .map(|f| face_normals[f.handle()])
                        .sum::<Vector3<f32>>()
                        .normalize();
                    (v.handle(), normal)
                }).collect::<VecMap<_, _>>()
            });

            Ok(out)
        }
    }
}

gen_calc_vertex_normals!(calc_vnormals_cat_hem, "cat.ply", HalfEdgeMesh<TriConfig>);
gen_calc_vertex_normals!(calc_vnormals_cat_dem, "cat.ply", DirectedEdgeMesh);
gen_calc_vertex_normals!(calc_vnormals_tiger_hem, "tiger.ply", HalfEdgeMesh<TriConfig>);
gen_calc_vertex_normals!(calc_vnormals_tiger_dem, "tiger.ply", DirectedEdgeMesh);
