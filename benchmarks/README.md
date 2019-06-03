Benchmarks
==========

This folder contains all benchmarks mentioned in the thesis.
A quick overview:

- `comparison/`: benchmarks comparing lox, PMP and OpenMesh.
- `data/`: test meshes.
- `memory-layout/`: benchmarking "Array of Structs" vs. "Struct of Arrays" memory layouts in various scenarios.
- `raw-results/`: contains the data measured by me and used in the thesis in various formats.
- `util/`: contains Rust scripts for certain things.


## Building benchmarks

The Rust benchmarks were compiled with `rustc 1.36.0-nightly (a784a8022 2019-05-09)`.
The `rust-toolchain` file in this folder should already set the correct compiler version if you are using `rustup`.
The version of `lox` is fixed to a specific commit within `Cargo.toml`.
You can build and run a single Rust benchmark with `cargo bench --bench <name>`.
See `Cargo.toml` for available benchmarks.
Without the `--bench` argument, all benchmarks are executed.

The C++ benchmarks are CMake projects and contain submodules for OpenMesh and PMP.
To run those benchmarks, first checkout all submodules (fixed to a specific commit) by running `git submodule update --init --recursive`.
Next, create a `build` folder inside of `comparison/OpenMesh` or `comparison/pmp`.
Change into that directory and run `cmake ..` followed by `make -j8`.
This creates an executable `om-bench`/`pmp-bench` file in the `build` directory that you can execute.
(Note: I compiled those benchmarks with `gcc (Ubuntu 7.4.0-1ubuntu1~18.04) 7.4.0`).

## Running benchmarks

All benchmark "runners", including `memory-layout` (Rust), `comparison/lox` (Rust), `om-bench` (C++) or `pmp-bench` (C++) work similar:
simply executing them will run all benchmarks inside of them.
By providing a command line argument, benchmarks can be filtered by name.
The Rust benchmark runners support additional CLI parameters, run them with `-h` for more information.

Many benchmarks have hardcoded paths to meshes which means that they need to run in a specific folder.
Particularly, all Rust benchmarks need to be executed from this `benchmarks` folder via `cargo bench`; the C++ benchmarks need to be executed from the `build` folder.

For some IO benchmarks, a RAM disk at `/mnt/ramdisk` is required.
The scripts `create-ramdisk.sh` and `destroy-ramdisk.sh` are provided to easily do that on Linux (and maybe MacOS?).

The `memory-layout` benchmark uses Criterion and outputs interesting values.
Raw data is written to `target/criterion`.
All other benchmarks just print the mean and standard deviation and are not as sophisticated as Criterion.
