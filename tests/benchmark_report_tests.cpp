#include <catch2/catch_test_macros.hpp>

#include "renderlooplab/benchmark_report.hpp"
#include "renderlooplab/frame_profiler.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

using namespace rll;

static BenchmarkReport makeReport()
{
    BenchmarkReport r;
    r.env.operatingSystem = "TestOS";
    r.env.compiler        = "TestCompiler";
    r.env.gpuVendor       = "TestVendor";
    r.env.gpuRenderer     = "TestRenderer";
    r.env.openglVersion   = "3.3.0";

    r.config.width         = 1280;
    r.config.height        = 720;
    r.config.sprites       = 500;
    r.config.seed          = 12345;
    r.config.fixedHz       = 60.0;
    r.config.vsync         = false;
    r.config.warmupFrames  = 120;
    r.config.measuredFrames= 300;

    r.stats.sampleCount   = 300;
    r.stats.mean          = 5.0;
    r.stats.median        = 4.8;
    r.stats.min           = 3.0;
    r.stats.max           = 12.0;
    r.stats.percentile95  = 7.0;
    r.stats.percentile99  = 9.0;
    r.stats.stddev        = 1.0;
    r.stats.avgFps        = 200.0;
    r.stats.avgDrawCalls  = 1.0;
    r.stats.avgTriangles  = 1000.0;

    return r;
}

TEST_CASE("BenchmarkReport: JSON contains required top-level keys", "[report]")
{
    const std::string path = "test_report_keys.json";
    auto report = makeReport();
    REQUIRE(writeBenchmarkJson(path, report));

    std::ifstream f(path);
    REQUIRE(f.is_open());
    std::ostringstream ss;
    ss << f.rdbuf();
    const std::string json = ss.str();

    REQUIRE(json.find("\"tool\"")          != std::string::npos);
    REQUIRE(json.find("\"tool_version\"")  != std::string::npos);
    REQUIRE(json.find("\"mode\"")          != std::string::npos);
    REQUIRE(json.find("\"environment\"")   != std::string::npos);
    REQUIRE(json.find("\"configuration\"") != std::string::npos);
    REQUIRE(json.find("\"timing_ms\"")     != std::string::npos);
    REQUIRE(json.find("\"average_fps\"")   != std::string::npos);
    REQUIRE(json.find("\"rendering\"")     != std::string::npos);

    f.close();
    std::filesystem::remove(path);
}

TEST_CASE("BenchmarkReport: JSON contains timing fields", "[report]")
{
    const std::string path = "test_report_timing.json";
    auto report = makeReport();
    REQUIRE(writeBenchmarkJson(path, report));

    std::ifstream f(path);
    std::ostringstream ss;
    ss << f.rdbuf();
    const std::string json = ss.str();

    REQUIRE(json.find("\"minimum\"")           != std::string::npos);
    REQUIRE(json.find("\"maximum\"")           != std::string::npos);
    REQUIRE(json.find("\"mean\"")              != std::string::npos);
    REQUIRE(json.find("\"median\"")            != std::string::npos);
    REQUIRE(json.find("\"percentile_95\"")     != std::string::npos);
    REQUIRE(json.find("\"percentile_99\"")     != std::string::npos);
    REQUIRE(json.find("\"standard_deviation\"")!= std::string::npos);

    f.close();
    std::filesystem::remove(path);
}

TEST_CASE("BenchmarkReport: JSON write to invalid path returns false", "[report]")
{
    auto report = makeReport();
    bool ok = writeBenchmarkJson("/no_such_dir/report.json", report);
    REQUIRE_FALSE(ok);
}

TEST_CASE("BenchmarkReport: JSON escapes special characters", "[report]")
{
    const std::string path = "test_report_escape.json";
    auto report = makeReport();
    report.env.gpuRenderer = "Renderer \"with quotes\" and \\ backslash";
    REQUIRE(writeBenchmarkJson(path, report));

    std::ifstream f(path);
    std::ostringstream ss;
    ss << f.rdbuf();
    const std::string json = ss.str();

    // Escaped characters must appear in the output
    REQUIRE(json.find("\\\"") != std::string::npos);
    REQUIRE(json.find("\\\\") != std::string::npos);

    f.close();
    std::filesystem::remove(path);
}

TEST_CASE("osIdentString returns non-empty string", "[report]")
{
    REQUIRE_FALSE(osIdentString().empty());
}

TEST_CASE("compilerIdentString returns non-empty string", "[report]")
{
    REQUIRE_FALSE(compilerIdentString().empty());
}
