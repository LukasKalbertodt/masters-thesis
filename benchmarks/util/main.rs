//! This small utility just collects relevant data from the `criterion` folder and combines it into
//! one csv file.

use serde::Deserialize;
use std::{
    env,
    error::Error,
    fs,
    path::Path,
    process::exit,
};

fn main() -> Result<(), Box<dyn Error>> {
    let arg = env::args().nth(1).expect("missing path to 'criterion' folder");
    let path = Path::new(&arg).canonicalize().expect("failed to canonicalize path");

    if !path.ends_with("criterion") || !path.is_dir() {
        eprintln!("Given path '{}' does not point to a folder called 'criterion'", arg);
        exit(1);
    }

    let mut lines = Vec::new();
    for entry in path.read_dir()? {
        let entry = entry?;

        // Skip everything that is not a benchmark folder
        let bench_path = entry.path();
        if !entry.file_type()?.is_dir() || bench_path.ends_with("report") {
            continue;
        }

        read_bench(&bench_path, &mut lines)?;
    }

    lines.sort_by(|a, b| (&a.name, &a.input).cmp(&(&b.name, &b.input)));

    println!("benchmark;input;mean;std dev");
    for line in lines {
        println!(
            "{};{};{};{}",
            line.name,
            line.input,
            // line.data.mean.confidence_interval.lower_bound,
            line.data.mean.point_estimate,
            // line.data.mean.confidence_interval.upper_bound,
            // line.data.std_dev.confidence_interval.lower_bound,
            line.data.std_dev.point_estimate,
            // line.data.std_dev.confidence_interval.upper_bound,
        );
    }

    Ok(())
}

#[derive(Debug)]
struct Line {
    name: String,
    input: String,
    data: Data,
}

fn read_bench(path: &Path, lines: &mut Vec<Line>) -> Result<(), Box<dyn Error>> {
    let name = path.file_name().unwrap().to_str().unwrap();

    for entry in path.read_dir()? {
        let entry = entry?;

        // Skip everything that is not an input folder
        let p = entry.path();
        if !entry.file_type()?.is_dir() || p.ends_with("report") {
            continue;
        }

        let input = p.file_name().unwrap().to_str().unwrap().to_string();
        let data = read_single_bench(&p)?;
        lines.push(Line { name: name.to_string(), input, data });
    }

    Ok(())
}

fn read_single_bench(path: &Path) -> Result<Data, Box<dyn Error>> {
    let file = fs::read_to_string(path.join("base").join("estimates.json"))?;
    let d = serde_json::from_str(&file)?;

    Ok(d)
}

#[derive(Debug, Deserialize)]
struct Confidence {
    confidence_level: f64,
    upper_bound: f64,
    lower_bound: f64,
}

#[derive(Debug, Deserialize)]
struct Estimate {
    point_estimate: f64,
    confidence_interval: Confidence,
}

#[derive(Debug, Deserialize)]
struct Data {
    #[serde(rename = "Mean")]
    mean: Estimate,

    #[serde(rename = "StdDev")]
    std_dev: Estimate,
}
