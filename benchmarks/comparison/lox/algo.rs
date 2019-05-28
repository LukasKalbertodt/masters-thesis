use failure::Error;
use libtest_mimic::Outcome;
use lox::{
    algo,
    ds::{
        DirectedEdgeMesh, HalfEdgeMesh,
        half_edge::TriConfig,
    },
    fat::MiniMesh,
    io,
};

use crate::util::do_bench;


macro_rules! gen_read_bench {
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

gen_read_bench!(smooth_cat_hem, "cat.ply", HalfEdgeMesh<TriConfig>);
gen_read_bench!(smooth_cat_dem, "cat.ply", DirectedEdgeMesh);
gen_read_bench!(smooth_tiger_hem, "tiger.ply", HalfEdgeMesh<TriConfig>);
gen_read_bench!(smooth_tiger_dem, "tiger.ply", DirectedEdgeMesh);
