use failure::Error;
use libtest_mimic::Outcome;
use lox::{
    ds::{
        DirectedEdgeMesh, HalfEdgeMesh, SharedVertexMesh,
        half_edge::TriConfig,
    },
    fat::MiniMesh,
    io,
};

use crate::util::do_bench;


macro_rules! gen_write_bench {
    ($fn_name:ident, $file:literal, $mesh:ty) => {
        pub fn $fn_name() -> Result<Outcome, Error> {
            let mesh: MiniMesh<$mesh> = io::read_file(&format!("data/{}", $file))?;

            let out = do_bench(|| {
                io::write_file(&mesh, &format!("/mnt/ramdisk/_out_{}", $file))
                    .expect("failed to write to ramdisk")
            });
            Ok(out)
        }
    }
}

// Tiger mesh
gen_write_bench!(ply_hem_tri_tiger, "tiger.ply", HalfEdgeMesh<TriConfig>);
gen_write_bench!(ply_dem_tiger,     "tiger.ply", DirectedEdgeMesh);
gen_write_bench!(ply_sv_tiger,      "tiger.ply", SharedVertexMesh);
gen_write_bench!(stl_hem_tri_tiger, "tiger.stl", HalfEdgeMesh<TriConfig>);
gen_write_bench!(stl_dem_tiger,     "tiger.stl", DirectedEdgeMesh);
gen_write_bench!(stl_sv_tiger,      "tiger.stl", SharedVertexMesh);

// Cat mesh
gen_write_bench!(ply_hem_tri_cat, "cat.ply", HalfEdgeMesh<TriConfig>);
gen_write_bench!(ply_dem_cat,     "cat.ply", DirectedEdgeMesh);
gen_write_bench!(ply_sv_cat,      "cat.ply", SharedVertexMesh);
gen_write_bench!(stl_hem_tri_cat, "cat.stl", HalfEdgeMesh<TriConfig>);
gen_write_bench!(stl_dem_cat,     "cat.stl", DirectedEdgeMesh);
gen_write_bench!(stl_sv_cat,      "cat.stl", SharedVertexMesh);
