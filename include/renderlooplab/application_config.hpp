#pragma once

#include <string>
#include <cstdint>

namespace rll {

/// All configuration that drives one application run.
/// Constructed by CLI parsing; passed to Application.
struct ApplicationConfig {
    // ── Window ──────────────────────────────────────────────────────────
    int  width  = 1280;
    int  height = 720;
    bool hidden = false;  ///< Headless / CI mode

    // ── Simulation ───────────────────────────────────────────────────────
    int      sprites  = 500;
    uint64_t seed     = 12345;
    double   fixedHz  = 60.0;

    // ── Rendering ────────────────────────────────────────────────────────
    bool vsync = true;

    // ── Run mode ─────────────────────────────────────────────────────────
    bool benchmark = false;
    bool smokeTest = false;

    // ── Benchmark / smoke-test parameters ────────────────────────────────
    int warmupFrames   = 120;
    int measuredFrames = 0;   ///< 0 = run until window is closed

    // ── Output ───────────────────────────────────────────────────────────
    std::string csvPath;
    std::string reportPath;

    // ── Asset discovery ───────────────────────────────────────────────────
    std::string assetRoot;  ///< Empty → auto-discover next to executable

    // ── Limits ────────────────────────────────────────────────────────────
    static constexpr int MIN_SPRITES     = 1;
    static constexpr int MAX_SPRITES     = 100000;
    static constexpr double MIN_FIXED_HZ = 1.0;
    static constexpr double MAX_FIXED_HZ = 500.0;
};

} // namespace rll
