#![feature(test)]

extern crate test;

use failure::Error;
use libtest_mimic::{Arguments, Test, Outcome, run_tests};

mod algo;
mod read_mem;
mod read_file;
mod write_mem;
mod write_file;
mod util;


fn main() {
    let args = Arguments::from_args();

    // Create list of all benchmarks
    macro_rules! bench {
        ($fun:path) => {
            Test {
                name: stringify!($fun).into(),
                kind: "".into(),
                is_ignored: false,
                is_bench: true,
                data: $fun as fn() -> Result<Outcome, Error>,
            }
        }
    }

    let benches = vec![
        // IO read benchmark
        bench!(read_mem::ply_hem_tri_tiger),
        bench!(read_mem::ply_dem_tiger),
        bench!(read_mem::ply_sv_tiger),
        bench!(read_mem::ply_hem_tri_cat),
        bench!(read_mem::ply_dem_cat),
        bench!(read_mem::ply_sv_cat),
        bench!(read_mem::stl_hem_tri_tiger),
        bench!(read_mem::stl_dem_tiger),
        bench!(read_mem::stl_sv_tiger),
        bench!(read_mem::stl_hem_tri_cat),
        bench!(read_mem::stl_dem_cat),
        bench!(read_mem::stl_sv_cat),

        bench!(read_file::ply_hem_tri_tiger),
        bench!(read_file::ply_dem_tiger),
        bench!(read_file::ply_sv_tiger),
        bench!(read_file::ply_hem_tri_cat),
        bench!(read_file::ply_dem_cat),
        bench!(read_file::ply_sv_cat),
        bench!(read_file::stl_hem_tri_tiger),
        bench!(read_file::stl_dem_tiger),
        bench!(read_file::stl_sv_tiger),
        bench!(read_file::stl_hem_tri_cat),
        bench!(read_file::stl_dem_cat),
        bench!(read_file::stl_sv_cat),

        // IO write benchmark
        bench!(write_mem::ply_hem_tri_tiger),
        bench!(write_mem::ply_dem_tiger),
        bench!(write_mem::ply_sv_tiger),
        bench!(write_mem::ply_hem_tri_cat),
        bench!(write_mem::ply_dem_cat),
        bench!(write_mem::ply_sv_cat),
        bench!(write_mem::stl_hem_tri_tiger),
        bench!(write_mem::stl_dem_tiger),
        bench!(write_mem::stl_sv_tiger),
        bench!(write_mem::stl_hem_tri_cat),
        bench!(write_mem::stl_dem_cat),
        bench!(write_mem::stl_sv_cat),

        bench!(write_file::ply_hem_tri_tiger),
        bench!(write_file::ply_dem_tiger),
        bench!(write_file::ply_sv_tiger),
        bench!(write_file::ply_hem_tri_cat),
        bench!(write_file::ply_dem_cat),
        bench!(write_file::ply_sv_cat),
        bench!(write_file::stl_hem_tri_tiger),
        bench!(write_file::stl_dem_tiger),
        bench!(write_file::stl_sv_tiger),
        bench!(write_file::stl_hem_tri_cat),
        bench!(write_file::stl_dem_cat),
        bench!(write_file::stl_sv_cat),

        // Algorithm
        bench!(algo::smooth_cat_hem),
        bench!(algo::smooth_cat_dem),
        bench!(algo::smooth_tiger_hem),
        bench!(algo::smooth_tiger_dem),

        bench!(algo::sqrt3_cat_hem),
        bench!(algo::sqrt3_tiger_hem),

        bench!(algo::boundary_vcount_cat_hem),
        bench!(algo::boundary_vcount_cat_dem),
        bench!(algo::boundary_vcount_tiger_hem),
        bench!(algo::boundary_vcount_tiger_dem),

        bench!(algo::boundary_fcount_cat_hem),
        bench!(algo::boundary_fcount_cat_dem),
        bench!(algo::boundary_fcount_tiger_hem),
        bench!(algo::boundary_fcount_tiger_dem),

        bench!(algo::calc_fnormals_cat_hem),
        bench!(algo::calc_fnormals_cat_dem),
        bench!(algo::calc_fnormals_cat_svm),
        bench!(algo::calc_fnormals_tiger_hem),
        bench!(algo::calc_fnormals_tiger_dem),
        bench!(algo::calc_fnormals_tiger_svm),

        bench!(algo::calc_vnormals_cat_hem),
        bench!(algo::calc_vnormals_cat_dem),
        bench!(algo::calc_vnormals_tiger_hem),
        bench!(algo::calc_vnormals_tiger_dem),
    ];


    // Run benchmarks
    run_tests(&args, benches, |bench| {
        let out = (bench.data)();
        out.unwrap_or_else(|e| Outcome::Failed { msg: Some(e.to_string()) })
    }).exit();
}
