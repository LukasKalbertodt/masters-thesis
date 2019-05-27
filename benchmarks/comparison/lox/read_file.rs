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


macro_rules! gen_read_bench {
    ($fn_name:ident, $format:ident, $file:literal, $mesh:ty) => {
        pub fn $fn_name() -> Result<Outcome, Error> {
            let out = do_bench(|| {
                io::read_file::<MiniMesh<$mesh>, _>(format!("/mnt/ramdisk/{}", $file))
                    .expect("failed to read from ramdisk")
            });
            Ok(out)
        }
    }
}

// Tiger mesh
gen_read_bench!(ply_hem_tri_tiger,  Ply, "tiger.ply", HalfEdgeMesh<TriConfig>);
gen_read_bench!(ply_dem_tiger,      Ply, "tiger.ply", DirectedEdgeMesh);
gen_read_bench!(ply_sv_tiger,       Ply, "tiger.ply", SharedVertexMesh);
gen_read_bench!(stl_hem_tri_tiger,  Stl, "tiger.stl", HalfEdgeMesh<TriConfig>);
gen_read_bench!(stl_dem_tiger,      Stl, "tiger.stl", DirectedEdgeMesh);
gen_read_bench!(stl_sv_tiger,       Stl, "tiger.stl", SharedVertexMesh);

// Cat mesh
gen_read_bench!(ply_hem_tri_cat,  Ply, "cat.ply", HalfEdgeMesh<TriConfig>);
gen_read_bench!(ply_dem_cat,      Ply, "cat.ply", DirectedEdgeMesh);
gen_read_bench!(ply_sv_cat,       Ply, "cat.ply", SharedVertexMesh);
gen_read_bench!(stl_hem_tri_cat,  Stl, "cat.stl", HalfEdgeMesh<TriConfig>);
gen_read_bench!(stl_dem_cat,      Stl, "cat.stl", DirectedEdgeMesh);
gen_read_bench!(stl_sv_cat,       Stl, "cat.stl", SharedVertexMesh);
