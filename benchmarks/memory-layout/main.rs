//! This benchmarks compares the memory layouts "Array of Structs" (AOS) with
//! "Struct of Arrays" (SOA) in several situations.
//!
//! The parameter space has four dimensions:
//! - Total number of items
//! - Number of fields per item/number of used fields per item
//! - Traversal kind: random or sequential
//! - Memory layout: AOS or SOA
//!
//! The first dimension is realized with criterions `bench_over_inputs`
//! feature. The second dimension is realized via a macro that gets these
//! parameters as inputs. The last two dimensions are programmed separately.
//!
//! There are many tricky things about this benchmark. See the written thesis
//! for more information.

use std::time::Duration;
use criterion::{
    criterion_group, criterion_main, black_box, Criterion,
};
use rand::{
    prelude::*,
    rngs::SmallRng,
};


/// Flushes all CPU caches.
///
/// This function requires a kernel module that actually flushes the caches
/// when `/proc/wbinvd` is read. The kernel module that was used for this
/// benchmark was: https://github.com/batmac/wbinvd
///
/// The actual flushing is done via the WBINVD instruction which is only
/// callable from ring0. Hence the kernel module.
///
/// It is actually not used in the benchmarks due to its large overhead.
#[inline(never)]
fn flush_cache() {
    let s = std::fs::read_to_string("/proc/wbinvd").expect(
        "Could not read '/proc/wbinvd': make sure you have the wbinvd \
            kernel module installed and run this benchmark as root!"
    );

    // It's very unlikely the read from that file will be optimized away, but
    // just to be sure:
    black_box(s);
}

/// Return a random number generator with a fixed seed. Thus it always
/// generates the same sequence of numbers. This is to keep benchmarks
/// consistent across runs.
fn get_rng() -> SmallRng {
    // Arbitrary but fixed seed.
    SmallRng::seed_from_u64(27)
}

/// We run all benchmarks for all of these "number of items".
const ITEM_COUNTS: &[usize] =
    &[10_000, 100_000, 1_000_000, 10_000_000];
    // &[10_000, 100_000, 1_000_000];
    // &[10_000_000];


/// Helper trait to abstract over different array lengths. Just used below to
/// randomly generated arrays.
trait RandomArray<T>: Sized {
    /// Calls `f` once for each array element and returns all results.
    fn from(f: impl FnMut() -> T) -> Self;
}

macro_rules! impl_random_array {
    ($($len:literal, |$f:ident| $e:expr ; )*) => {
        $(
            impl<T> RandomArray<T> for [T; $len] {
                fn from(mut $f: impl FnMut() -> T) -> Self {
                    $e
                }
            }
        )*
    }
}

impl_random_array!(
    1, |f| [f()];
    2, |f| [f(), f()];
    4, |f| [f(), f(), f(), f()];
    6, |f| [f(), f(), f(), f(), f(), f()];
);


/// The macro that generates the actual benchmarks.
///
/// It gets the number of fields and number of used fields as input. A separate
/// module is generated for these configurations.
macro_rules! gen_benchmarks {
    ($modname:ident, $num_fields:literal, $used_fields:literal) => {
        mod $modname {
            use std::iter;
            use criterion::{BatchSize, Criterion};
            use rand::prelude::*;
            use super::{ITEM_COUNTS, get_rng, RandomArray};

            /// Abstracts over the data layouts (AOS/SOA).
            trait Data: Sized {
                /// Returns a random instance with the given number of items.
                /// The first field of all items is an index to another item:
                /// thus it has to be <= `num_items`, but is still random.
                fn random(num_items: usize) -> Self;

                /// Returns the number of items.
                fn num_items(&self) -> usize;

                /// Returns the value of the field with the given field index
                /// of the item with the given item index.
                fn get(&self, item: usize, field: usize) -> u32;
            }

            // ---------------------------------------------------------------
            // ----- Array of structs
            // ---------------------------------------------------------------
            #[derive(Clone)]
            struct Aos(Vec<[u32; $num_fields]>);

            impl Data for Aos {
                #[inline(never)]
                fn random(num_items: usize) -> Self {
                    let mut rng = get_rng();
                    let mut v: Vec<[u32; $num_fields]>
                        = iter::repeat_with(|| rng.gen()).take(num_items).collect();
                    let len = v.len();

                    for arr in &mut v {
                        arr[0] = arr[0] % (len as u32);
                    }

                    Self(v)
                }

                #[inline(always)]
                fn num_items(&self) -> usize {
                    self.0.len()
                }

                #[inline(always)]
                fn get(&self, item: usize, field: usize) -> u32 {
                    unsafe { *self.0.get_unchecked(item).get_unchecked(field) }
                }
            }

            // ---------------------------------------------------------------
            // ----- Struct of arrays
            // ---------------------------------------------------------------
            #[derive(Clone)]
            struct Soa([Vec<u32>; $num_fields]);

            impl Data for Soa {
                #[inline(never)]
                fn random(num_items: usize) -> Self {
                    let mut rng = get_rng();
                    let random_vector = || iter::repeat_with(|| rng.gen())
                        .take(num_items)
                        .collect::<Vec<u32>>();
                    let mut arr: [Vec<u32>; $num_fields] = RandomArray::from(random_vector);

                    let len = arr[0].len();

                    for v in &mut arr[0] {
                        *v = *v % (len as u32);
                    }

                    Self(arr)
                }

                #[inline(always)]
                fn num_items(&self) -> usize {
                    self.0[0].len()
                }

                #[inline(always)]
                fn get(&self, item: usize, field: usize) -> u32 {
                    unsafe { *self.0.get_unchecked(field).get_unchecked(item) }
                }
            }


            // ----------------------------------------------------------------

            /// Runs the actual benchmark code for random traversal. Uses the
            /// first field of every item as next index. Returns the sum (as
            /// dummy value) over all other fields that are used.
            #[inline(never)]
            fn run_random(data: &impl Data) -> u64 {
                let mut sum = 0u64;
                let count = data.num_items();
                let mut idx = 0;

                for _ in 0..count {
                    idx = data.get(idx, 0) as usize;
                    for field in 0..$used_fields {
                        sum += data.get(idx, field) as u64;
                    }
                }

                sum
            }

            /// Runs the actual benchmark code for sequential traversal. Simply
            /// iterrates over all indices. Returns the sum (as dummy value)
            /// over all fields that are used.
            #[inline(never)]
            fn run_seq(data: &impl Data) -> u64 {
                let mut sum = 0u64;

                for item in 0..data.num_items() {
                    for field in 0..$used_fields {
                        sum += data.get(item, field) as u64;
                    }
                }

                sum
            }

            /// Adds the benchmarks to Criterion.
            pub fn benchmarks(c: &mut Criterion) {
                macro_rules! gen {
                    ($data:ident, $traversal:ident, $fun:ident) => {
                        let name = format!(
                            "{}::main_{}_{}",
                            stringify!($modname),
                            stringify!($data).to_lowercase(),
                            stringify!($traversal),
                        );

                        c.bench_function_over_inputs(
                            &name,
                            |b, length| {
                                // We may use up to 4 GB of RAM.
                                let batch_size = 4_000_000_000u64
                                    / (*length as u64 * 4 * $num_fields);
                                let data = $data::random(*length);

                                b.iter_batched_ref(
                                    || data.clone(),
                                    |data| $fun(&*data),
                                    BatchSize::NumIterations(batch_size),
                                )
                            },
                            ITEM_COUNTS.to_vec(),
                        );
                    }
                }

                /// Generate benchmarks for all configurations of data layout
                /// and traversal.
                gen!(Aos, seq, run_seq);
                gen!(Aos, random, run_random);
                gen!(Soa, seq, run_seq);
                gen!(Soa, random, run_random);
            }
        }
    }
}

/// Generate benchmarks for all configurations of number of fields/used fields.
gen_benchmarks!(f1_1, 1, 1);
gen_benchmarks!(f2_1, 2, 1);
gen_benchmarks!(f2_2, 2, 2);
gen_benchmarks!(f4_1, 4, 1);
gen_benchmarks!(f4_2, 4, 2);
gen_benchmarks!(f4_4, 4, 4);
gen_benchmarks!(f6_1, 6, 1);
gen_benchmarks!(f6_3, 6, 3);
gen_benchmarks!(f6_6, 6, 6);


/// Just a baseline benchmark to know how much of the time is used by clearing
/// the cache. This turned out to be too high.
fn clear_cache(c: &mut Criterion) {
    c.bench_function("clear_cache", |b| {
        b.iter(|| flush_cache())
    });
}

/// Register all benchmarks.
criterion_group!(util, clear_cache);
criterion_group!(
    name = benches;
    // We use values slightly below the default, because some benchmarks take a
    // very long time. Even with this configuration, running the whole suite
    // took 3 hours on my notebook.
    config = Criterion::default()
        .warm_up_time(Duration::from_secs(2))
        .measurement_time(Duration::from_secs(5))
        .sample_size(50);
    targets =
        f1_1::benchmarks,
        f2_1::benchmarks,
        f2_2::benchmarks,
        f4_1::benchmarks,
        f4_2::benchmarks,
        f4_4::benchmarks,
        f6_1::benchmarks,
        f6_3::benchmarks,
        f6_6::benchmarks,
);
criterion_main!(util, benches);
