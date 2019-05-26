#![feature(test)]

extern crate test;

use failure::Error;
use libtest_mimic::{Arguments, Test, Outcome, run_tests};

mod read;
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
        bench!(read::ply_hem_tri_tiger),
        bench!(read::ply_dem_tiger),
        bench!(read::ply_sv_tiger),
        bench!(read::stl_hem_tri_tiger),
        bench!(read::stl_dem_tiger),
        bench!(read::stl_sv_tiger),
        bench!(read::ply_hem_tri_cat),
        bench!(read::ply_dem_cat),
        bench!(read::ply_sv_cat),
        bench!(read::stl_hem_tri_cat),
        bench!(read::stl_dem_cat),
        bench!(read::stl_sv_cat),
    ];


    // Run benchmarks
    run_tests(&args, benches, |bench| {
        let out = (bench.data)();
        out.unwrap_or_else(|e| Outcome::Failed { msg: Some(e.to_string()) })
    }).exit();
}
