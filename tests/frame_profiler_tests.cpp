#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "renderlooplab/frame_profiler.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace rll;
using Approx = Catch::Approx;

// Helper: build a profiler with known total-frame times
static FrameProfiler makeProfiler(const std::vector<double>& totalMsValues)
{
    FrameProfiler p;
    uint64_t frame = 0;
    for (double v : totalMsValues) {
        FrameSample s;
        s.frame      = frame++;
        s.totalMs    = v;
        s.drawCalls  = 1;
        s.sprites    = 100;
        p.addSample(s);
    }
    return p;
}

TEST_CASE("FrameProfiler: empty samples return zero stats", "[profiler]")
{
    FrameProfiler p;
    auto st = p.computeStats();
    REQUIRE(st.sampleCount == 0);
    REQUIRE(st.mean        == Approx(0.0));
    REQUIRE(st.min         == Approx(0.0));
    REQUIRE(st.max         == Approx(0.0));
}

TEST_CASE("FrameProfiler: single sample", "[profiler]")
{
    auto p = makeProfiler({16.0});
    auto st = p.computeStats();
    REQUIRE(st.sampleCount  == 1);
    REQUIRE(st.mean         == Approx(16.0));
    REQUIRE(st.min          == Approx(16.0));
    REQUIRE(st.max          == Approx(16.0));
    REQUIRE(st.median       == Approx(16.0));
    REQUIRE(st.stddev       == Approx(0.0));
    REQUIRE(st.avgFps       == Approx(1000.0 / 16.0).epsilon(1e-5));
}

TEST_CASE("FrameProfiler: mean is arithmetic mean", "[profiler]")
{
    auto p = makeProfiler({10.0, 20.0, 30.0});
    auto st = p.computeStats();
    REQUIRE(st.mean == Approx(20.0));
}

TEST_CASE("FrameProfiler: median odd sample count", "[profiler]")
{
    auto p = makeProfiler({30.0, 10.0, 20.0});  // sorted: 10, 20, 30
    auto st = p.computeStats();
    REQUIRE(st.median == Approx(20.0));
}

TEST_CASE("FrameProfiler: median even sample count", "[profiler]")
{
    auto p = makeProfiler({10.0, 20.0, 30.0, 40.0});  // sorted: 10,20,30,40
    auto st = p.computeStats();
    REQUIRE(st.median == Approx(25.0));
}

TEST_CASE("FrameProfiler: min and max", "[profiler]")
{
    auto p = makeProfiler({5.0, 15.0, 3.0, 25.0, 7.0});
    auto st = p.computeStats();
    REQUIRE(st.min == Approx(3.0));
    REQUIRE(st.max == Approx(25.0));
}

TEST_CASE("FrameProfiler: standard deviation", "[profiler]")
{
    // Known: values 2,4,4,4,5,5,7,9 → mean=5, population σ=2
    auto p = makeProfiler({2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0});
    auto st = p.computeStats();
    REQUIRE(st.mean   == Approx(5.0));
    REQUIRE(st.stddev == Approx(2.0).epsilon(1e-5));
}

TEST_CASE("FrameProfiler: 95th percentile nearest-rank", "[profiler]")
{
    // 20 equal values of 1..20
    std::vector<double> vals;
    for (int i = 1; i <= 20; ++i)
        vals.push_back(static_cast<double>(i));
    auto p = makeProfiler(vals);
    auto st = p.computeStats();
    // ceil(95/100 * 20) - 1 = 19 - 1 = 18  → sorted[18] = 19
    REQUIRE(st.percentile95 == Approx(19.0));
}

TEST_CASE("FrameProfiler: 99th percentile nearest-rank", "[profiler]")
{
    // 100 values 1..100
    std::vector<double> vals;
    for (int i = 1; i <= 100; ++i)
        vals.push_back(static_cast<double>(i));
    auto p = makeProfiler(vals);
    auto st = p.computeStats();
    // ceil(99/100 * 100) - 1 = 99 - 1 = 98 → sorted[98] = 99
    REQUIRE(st.percentile99 == Approx(99.0));
}

TEST_CASE("FrameProfiler: avgFps is 1000/mean", "[profiler]")
{
    auto p = makeProfiler({10.0, 10.0, 10.0});
    auto st = p.computeStats();
    REQUIRE(st.avgFps == Approx(100.0).epsilon(1e-5));
}

TEST_CASE("FrameProfiler: CSV output has correct header", "[profiler]")
{
    auto p = makeProfiler({16.0, 17.0});
    const std::string path = "test_profiler_output.csv";
    REQUIRE(p.writeCsv(path));

    std::ifstream f(path);
    REQUIRE(f.is_open());
    std::string header;
    std::getline(f, header);
    REQUIRE(header == "frame,input_ms,update_ms,render_ms,swap_ms,total_ms,updates,draw_calls,sprites");

    // Clean up (close before remove — Windows requires no open handles)
    f.close();
    std::filesystem::remove(path);
}

TEST_CASE("FrameProfiler: CSV has correct number of data rows", "[profiler]")
{
    auto p = makeProfiler({1.0, 2.0, 3.0, 4.0, 5.0});
    const std::string path = "test_profiler_rows.csv";
    REQUIRE(p.writeCsv(path));

    std::ifstream f(path);
    int lines = 0;
    std::string line;
    while (std::getline(f, line)) ++lines;
    // 1 header + 5 data rows = 6
    REQUIRE(lines == 6);

    f.close();
    std::filesystem::remove(path);
}

TEST_CASE("FrameProfiler: invalid CSV path returns false", "[profiler]")
{
    auto p = makeProfiler({1.0});
    // Non-existent directory
    bool ok = p.writeCsv("/nonexistent_dir/test.csv");
    REQUIRE_FALSE(ok);
}
