#pragma once

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <fast_io/fast_io_dsal/string.h>
#include <fast_io/fast_io_dsal/string_view.h>
#include <fast_io/fast_io.h>
#include <numeric>
#include <vector>

namespace pltxt2htm_bench {

struct BenchResult {
    ::std::size_t input_bytes{};
    ::std::size_t output_bytes{};
    ::std::size_t iterations{};
    ::std::chrono::nanoseconds total_ns{0};
    ::std::chrono::nanoseconds min_ns{::std::chrono::nanoseconds::max()};
    ::std::chrono::nanoseconds max_ns{0};
    ::std::chrono::nanoseconds median_ns{0};
};

inline void do_not_optimize(volatile void* p) noexcept {
    (void)p;
}

inline volatile ::std::uintptr_t sink;

template<typename F>
[[nodiscard]]
inline BenchResult benchmark(F&& func, ::std::size_t input_bytes = 0, ::std::size_t output_bytes = 0, int warmup = 5,
                             int iterations = 15) noexcept {
    for (int i = 0; i < warmup; ++i) {
        auto result = func();
        sink = reinterpret_cast<::std::uintptr_t>(result.data());
    }

    ::std::vector<::std::chrono::nanoseconds> samples;
    samples.reserve(static_cast<::std::size_t>(iterations));

    for (int i = 0; i < iterations; ++i) {
        auto start = ::std::chrono::high_resolution_clock::now();
        auto result = func();
        auto end = ::std::chrono::high_resolution_clock::now();
        sink = reinterpret_cast<::std::uintptr_t>(result.data());
        samples.push_back(::std::chrono::duration_cast<::std::chrono::nanoseconds>(end - start));
    }

    BenchResult res;
    res.input_bytes = input_bytes;
    res.output_bytes = output_bytes;
    res.iterations = static_cast<::std::size_t>(iterations);

    for (auto s : samples) {
        res.total_ns += s;
        if (s < res.min_ns)
            res.min_ns = s;
        if (s > res.max_ns)
            res.max_ns = s;
    }

    ::std::sort(samples.begin(), samples.end());
    res.median_ns = samples[samples.size() / 2];

    return res;
}

template<typename F>
[[nodiscard]]
inline BenchResult benchmark_void(F&& func, ::std::size_t input_bytes = 0, int warmup = 5,
                                  int iterations = 15) noexcept {
    for (int i = 0; i < warmup; ++i) {
        func();
    }

    ::std::vector<::std::chrono::nanoseconds> samples;
    samples.reserve(static_cast<::std::size_t>(iterations));

    for (int i = 0; i < iterations; ++i) {
        auto start = ::std::chrono::high_resolution_clock::now();
        func();
        auto end = ::std::chrono::high_resolution_clock::now();
        samples.push_back(::std::chrono::duration_cast<::std::chrono::nanoseconds>(end - start));
    }

    BenchResult res;
    res.input_bytes = input_bytes;
    res.iterations = static_cast<::std::size_t>(iterations);

    for (auto s : samples) {
        res.total_ns += s;
        if (s < res.min_ns)
            res.min_ns = s;
        if (s > res.max_ns)
            res.max_ns = s;
    }

    ::std::sort(samples.begin(), samples.end());
    res.median_ns = samples[samples.size() / 2];

    return res;
}

inline void print_result(::fast_io::u8string_view name, ::fast_io::u8string_view config,
                         BenchResult const& res) noexcept {
    auto median_us = ::std::chrono::duration_cast<::std::chrono::duration<double, ::std::micro>>(res.median_ns).count();
    auto min_us = ::std::chrono::duration_cast<::std::chrono::duration<double, ::std::micro>>(res.min_ns).count();
    auto max_us = ::std::chrono::duration_cast<::std::chrono::duration<double, ::std::micro>>(res.max_ns).count();
    auto total_ms = ::std::chrono::duration_cast<::std::chrono::duration<double, ::std::milli>>(res.total_ns).count();

    ::fast_io::io::print(::fast_io::u8out(), u8"  ", name, u8" [", config, u8"]\n");
    ::fast_io::io::print(::fast_io::u8out(), u8"    Iterations: ", static_cast<::std::size_t>(res.iterations), u8"\n");

    if (res.input_bytes > 0) {
        double throughput = static_cast<double>(res.input_bytes) * res.iterations / (total_ms / 1000.0);
        ::fast_io::io::print(::fast_io::u8out(), u8"    Input:      ", static_cast<::std::size_t>(res.input_bytes),
                             u8" bytes, Output: ", static_cast<::std::size_t>(res.output_bytes), u8" bytes\n");
        ::fast_io::io::print(::fast_io::u8out(), u8"    Throughput: ", throughput / 1024.0 / 1024.0, u8" MB/s\n");

        if (res.output_bytes > 0) {
            double output_throughput = static_cast<double>(res.output_bytes) * res.iterations / (total_ms / 1000.0);
            ::fast_io::io::print(::fast_io::u8out(), u8"    Output throughput: ", output_throughput / 1024.0 / 1024.0,
                                 u8" MB/s\n");
        }
    }

    ::fast_io::io::print(::fast_io::u8out(), u8"    Median:     ", median_us, u8" us\n");
    ::fast_io::io::print(::fast_io::u8out(), u8"    Min:        ", min_us, u8" us\n");
    ::fast_io::io::print(::fast_io::u8out(), u8"    Max:        ", max_us, u8" us\n");
    ::fast_io::io::print(::fast_io::u8out(), u8"\n");
}

inline void print_result_csv(::fast_io::u8string_view name, ::fast_io::u8string_view config,
                             BenchResult const& res) noexcept {
    auto median_us = ::std::chrono::duration_cast<::std::chrono::duration<double, ::std::micro>>(res.median_ns).count();
    auto min_us = ::std::chrono::duration_cast<::std::chrono::duration<double, ::std::micro>>(res.min_ns).count();
    auto max_us = ::std::chrono::duration_cast<::std::chrono::duration<double, ::std::micro>>(res.max_ns).count();
    auto total_ms = ::std::chrono::duration_cast<::std::chrono::duration<double, ::std::milli>>(res.total_ns).count();

    double throughput = 0.0;
    if (res.input_bytes > 0 && total_ms > 0) {
        throughput = static_cast<double>(res.input_bytes) * res.iterations / (total_ms / 1000.0);
    }

    ::fast_io::io::print(::fast_io::u8out(), name, u8",", config, u8",", static_cast<::std::size_t>(res.iterations),
                         u8",", static_cast<::std::size_t>(res.input_bytes), u8",",
                         static_cast<::std::size_t>(res.output_bytes), u8",", total_ms, u8",", median_us, u8",", min_us,
                         u8",", max_us, u8",", throughput / 1024.0 / 1024.0, u8"\n");
}

inline void print_csv_header() noexcept {
    ::fast_io::io::print(
        ::fast_io::u8out(),
        u8"bench,config,iterations,input_bytes,output_bytes,total_ms,median_us,min_us,max_us,throughput_mbs\n");
}

} // namespace pltxt2htm_bench
