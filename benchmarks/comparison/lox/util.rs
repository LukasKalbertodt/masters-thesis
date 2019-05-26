use std::{
    fs,
    time::{Duration, Instant},
};
use test::black_box;

use failure::{Error, ResultExt};
use libtest_mimic::Outcome;


/// Loads the given file into a byte vector.
pub fn load_file(path: &str) -> Result<Vec<u8>, Error> {
    let error = format!(
        "could not load benchmark data '{}' (make sure to execute this benchmark \
            from the `benchmarks` folder)",
        path,
    );

    fs::read(path).context(error).map_err(|e| e.into())
}

/// Runs the given function multiple times and returns the mean and standard
/// deviation of all runs.
pub fn do_bench<R>(mut bench: impl FnMut() -> R) -> Outcome {
    const ITERS: usize = 100;


    // Warmup
    for _ in 0..ITERS / 5 {
        black_box(bench());
    }

    // Actually measuring
    let mut durations = Vec::with_capacity(ITERS);
    for _ in 0..ITERS {
        let before = Instant::now();
        let out = bench();
        durations.push(before.elapsed());

        black_box(out);
    }

    // Calculate mean and standard deviation
    let total_time = durations.iter().sum::<Duration>().as_nanos() as f64;
    let mean = total_time / ITERS as f64;
    let std_dev = {
        let squared_diffs = durations.iter()
            .map(|&x| ((x.as_nanos() as f64) - mean).powi(2))
            .sum::<f64>();
        (squared_diffs / ITERS as f64).sqrt()
    };


    Outcome::Measured {
        avg: mean as u64,
        variance: std_dev as u64,
    }
}
