#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "renderlooplab/fixed_timestep.hpp"

#include <thread>
#include <chrono>

using namespace rll;
using Approx = Catch::Approx;

TEST_CASE("FixedTimestep construction", "[fixed_timestep]")
{
    FixedTimestep ts(60.0);
    REQUIRE(ts.fixedDelta() == Approx(1.0 / 60.0).epsilon(1e-9));
}

TEST_CASE("FixedTimestep: alpha is in [0,1)", "[fixed_timestep]")
{
    FixedTimestep ts(60.0);
    ts.start();
    // Tick immediately (near-zero elapsed time)
    auto r = ts.tick();
    REQUIRE(r.alpha >= 0.0);
    REQUIRE(r.alpha <  1.0);
}

TEST_CASE("FixedTimestep: at least one update after one fixed delta", "[fixed_timestep]")
{
    FixedTimestep ts(60.0, 0.25, 5);
    ts.start();
    // Sleep a tiny bit longer than one fixed step
    std::this_thread::sleep_for(std::chrono::milliseconds(20)); // ~16.7ms fixed
    auto r = ts.tick();
    REQUIRE(r.numUpdates >= 1);
}

TEST_CASE("FixedTimestep: maxUpdates caps the update count", "[fixed_timestep]")
{
    // Very high fixed-Hz so many steps are needed; cap at 3
    FixedTimestep ts(1000.0, 1.0, 3);
    ts.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto r = ts.tick();
    REQUIRE(r.numUpdates <= 3);
}

TEST_CASE("FixedTimestep: frame time is clamped to maxDelta", "[fixed_timestep]")
{
    const double maxDelta = 0.05;   // 50ms
    FixedTimestep ts(60.0, maxDelta, 5);
    ts.start();
    // Sleep well beyond maxDelta (300ms)
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    auto r = ts.tick();
    // clamped frame time must not exceed maxDelta
    REQUIRE(r.frameTimeSec <= maxDelta + 1e-6);
}

TEST_CASE("FixedTimestep: frameTimeSec is non-negative", "[fixed_timestep]")
{
    FixedTimestep ts(60.0);
    ts.start();
    auto r = ts.tick();
    REQUIRE(r.frameTimeSec >= 0.0);
}

TEST_CASE("FixedTimestep: start resets accumulator", "[fixed_timestep]")
{
    FixedTimestep ts(60.0);
    ts.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ts.start();  // restart
    auto r = ts.tick();
    // After restart the accumulator is fresh; updates should be modest
    REQUIRE(r.numUpdates <= 5);
}
