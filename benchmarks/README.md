Benchmarks
==========

This folder contains all benchmarks mentioned in the thesis.
A quick overview:

- `memory-layout/`: testing "Array of Structs" vs. "Struct of Arrays" memory layout in various scenarios.
- ...


## Running benchmarks

The Rust benchmarks were compiled with `rustc 1.36.0-nightly (a784a8022 2019-05-09)`.
The `rust-toolchain` file in this folder should already set the correct compiler version if you are using `rustup`.
The version of `lox` is fixed to a specific commit within `Cargo.toml`.
You can run a single Rust benchmark with `cargo bench --bench <name>`.
Without the `--bench` argument, all benchmarks are executed.

One specific benchmark in the `memory-layout` benchmark additionally only runs on Linux and requires [this kernel module](https://github.com/batmac/wbinvd).
This is to explicitly flush CPU caches.
To not run this benchmark simply execute `cargo bench --bench memory-layout -- main`.
To execute this benchmark, run `cargo bench --bench memory-layout -- flush` as root user.
