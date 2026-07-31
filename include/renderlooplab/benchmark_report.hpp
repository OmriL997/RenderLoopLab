#pragma once

#include "renderlooplab/application_config.hpp"
#include "renderlooplab/frame_profiler.hpp"

#include <string>

namespace rll {

struct EnvironmentInfo {
    std::string operatingSystem;
    std::string compiler;
    std::string gpuVendor;
    std::string gpuRenderer;
    std::string openglVersion;
};

struct BenchmarkReport {
    EnvironmentInfo   env;
    ApplicationConfig config;
    ProfilerStats     stats;
};

/// Print a concise human-readable summary to stdout.
void printBenchmarkSummary(const BenchmarkReport& report);

/// Write a JSON report to the given file path.
/// Returns false and logs if writing fails.
bool writeBenchmarkJson(const std::string& path, const BenchmarkReport& report);

/// Build the compile-time compiler identification string.
std::string compilerIdentString();

/// Build a short OS identification string from preprocessor macros.
std::string osIdentString();

} // namespace rll
