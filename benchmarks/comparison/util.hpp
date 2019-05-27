#ifndef UTIL_HPP
#define UTIL_HPP

#include<chrono>
#include<cmath>
#include<fstream>
#include<iomanip>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>

using std::string;
using std::cout;
using std::endl;
using std::flush;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;



struct Outcome {
    uint64_t mean;
    uint64_t std_dev;

    Outcome(uint64_t mean, uint64_t std_dev) : mean(mean), std_dev(std_dev) {}
};

struct Bench {
    string name;
    Outcome (*fn)();

    Bench(string name, Outcome (*fn)()): name(name), fn(fn) {}
};

// Macro to avoid repeating the name
#define BENCH(name) Bench(#name, name)


// Formats the given number with comma as thousand separator and a trailing "ns".
string format_ns(uint64_t v) {
    // Special case
    if (v == 0) {
        return "0ns";
    }

    auto out = string();

    while (v >= 1000) {
        auto ones = static_cast<char>(((v / 1) % 10) + '0');
        auto tens = static_cast<char>(((v / 10) % 10) + '0');
        auto hundreds = static_cast<char>(((v / 100) % 10) + '0');

        out = string(",") + hundreds + tens + ones + out;

        v /= 1000;
    }

    while (v > 0) {
        out = static_cast<char>((v % 10) + '0') + out;
        v /= 10;
    }

    return out + "ns";
}

// Runs all benchmarks, applying name filters if given, and prints the outcome.
void run_all_benches(const int argc, const char* argv[], const vector<Bench> benches) {
    const size_t WIDTH = 25;

    cout << endl;

    for (auto& bench: benches) {
        // Filter benchmark by name
        if (argc > 1) {
            if (bench.name.find(argv[1]) == string::npos) {
                continue;
            }
        }

        cout << "   Running '" << bench.name << "' ..." << flush;

        // Execute the benchmark.
        const auto outcome = bench.fn();

        // Padding (assuming ASCII names)
        const auto padding = bench.name.size() > WIDTH ? 0 : WIDTH - bench.name.size();
        for (size_t i = 0; i < padding; i++) {
            cout << " ";
        }

        cout << format_ns(outcome.mean)
             << "  (std dev: "
             << format_ns(outcome.std_dev)
             << ")"
             << endl;
    }
}


// Number of iterations per benchmark
const int ITERS = 100;

// This is used to turn off compiler optimizations. A good explanation of this
// is here: https://youtu.be/nXaxk27zwlk?t=2440
template <typename T>
void black_box(T const& val) {
    asm volatile("" : : "g"(val) : "memory");
}

template <typename F>
Outcome run_bench(F bench) {
    // Warmup
    for (int i = 0; i < ITERS / 5; i++) {
        black_box(bench());
    }

    // Actually measuring.
    //
    // We measure each iteration individually. We can do that because all
    // benchmarks take around a milli second or way longer. The overhead/delay
    // by the measurement is not relevant in those time scales.
    auto durations = vector<uint64_t>();
    durations.reserve(ITERS);
    for (int i = 0; i < ITERS; i++) {
        const auto before = high_resolution_clock::now();
        const auto out = bench();
        const auto after = high_resolution_clock::now();

        black_box(out);
        const auto duration = after - before;
        const auto ns = duration_cast<std::chrono::nanoseconds>(duration).count();
        durations.push_back(ns);
    }

    // Calculate mean and standard deviation
    uint64_t total_time = 0;
    for (auto& x: durations) {
        total_time += x;
    }

    const auto mean = static_cast<double>(total_time) / static_cast<double>(ITERS);
    double squared_diffs = 0.0;
    for (auto& x: durations) {
        auto diff = static_cast<double>(x) - mean;
        squared_diffs += diff * diff;
    }

    const auto std_dev = sqrt(squared_diffs / static_cast<double>(ITERS));


    return Outcome(static_cast<uint64_t>(mean), static_cast<uint64_t>(std_dev));
}

// Reads a file completely into memory
std::string read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    file.unsetf(std::ios::skipws);

    std::string out;
    out.insert(
        out.begin(),
        std::istream_iterator<unsigned char>(file),
        std::istream_iterator<unsigned char>()
    );

    return out;
}



#endif // UTIL_HPP
