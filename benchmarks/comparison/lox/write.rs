use failure::Error;
use libtest_mimic::Outcome;
use lox::{
    ds::{
        DirectedEdgeMesh, HalfEdgeMesh, SharedVertexMesh,
        half_edge::TriConfig,
    },
    fat::MiniMesh,
    io::{self, FileFormat},
};

use crate::util::do_bench;


macro_rules! gen_write_bench {
    ($fn_name:ident, $format:ident, $file:literal, $mesh:ty) => {
        pub fn $fn_name() -> Result<Outcome, Error> {
            let mesh: MiniMesh<$mesh> = io::read_file(&format!("data/{}", $file))?;

            let out = do_bench(|| {
                let mut out = Vec::new();
                let r = io::write_to(FileFormat::$format, &mesh, &mut out);
                (r, out)
            });
            Ok(out)
        }
    }
}

// Tiger mesh
gen_write_bench!(ply_hem_tri_tiger,  Ply, "tiger.ply", HalfEdgeMesh<TriConfig>);
gen_write_bench!(ply_dem_tiger,      Ply, "tiger.ply", DirectedEdgeMesh);
gen_write_bench!(ply_sv_tiger,       Ply, "tiger.ply", SharedVertexMesh);
gen_write_bench!(stl_hem_tri_tiger,  Stl, "tiger.ply", HalfEdgeMesh<TriConfig>);
gen_write_bench!(stl_dem_tiger,      Stl, "tiger.ply", DirectedEdgeMesh);
gen_write_bench!(stl_sv_tiger,       Stl, "tiger.ply", SharedVertexMesh);

// Cat mesh
gen_write_bench!(ply_hem_tri_cat,  Ply, "cat.ply", HalfEdgeMesh<TriConfig>);
gen_write_bench!(ply_dem_cat,      Ply, "cat.ply", DirectedEdgeMesh);
gen_write_bench!(ply_sv_cat,       Ply, "cat.ply", SharedVertexMesh);
gen_write_bench!(stl_hem_tri_cat,  Stl, "cat.ply", HalfEdgeMesh<TriConfig>);
gen_write_bench!(stl_dem_cat,      Stl, "cat.ply", DirectedEdgeMesh);
gen_write_bench!(stl_sv_cat,       Stl, "cat.ply", SharedVertexMesh);
