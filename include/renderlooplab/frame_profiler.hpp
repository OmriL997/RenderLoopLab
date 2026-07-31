#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace rll {

/// Per-frame timing breakdown collected by FrameProfiler.
struct FrameSample {
    uint64_t frame      = 0;
    double   inputMs    = 0.0;
    double   updateMs   = 0.0;
    double   renderMs   = 0.0;
    double   swapMs     = 0.0;
    double   totalMs    = 0.0;
    int      numUpdates = 0;
    int      drawCalls  = 0;
    int      sprites    = 0;
};

/// Aggregate statistics computed from a collection of FrameSamples.
///
/// Percentile method: nearest-rank (lower-bound).
/// For N samples sorted ascending, the p-th percentile index is
///   ceil(p/100 * N) − 1  (clamped to [0, N-1]).
struct ProfilerStats {
    int    sampleCount   = 0;
    double min           = 0.0;
    double max           = 0.0;
    double mean          = 0.0;
    double median        = 0.0;
    double percentile95  = 0.0;
    double percentile99  = 0.0;
    double stddev        = 0.0;
    double avgFps        = 0.0;
    double avgDrawCalls  = 0.0;
    double avgTriangles  = 0.0;
};

/**
 * Lightweight frame profiler.
 *
 * Usage per frame:
 *   profiler.beginFrame();
 *   // ... event processing ...
 *   profiler.markInputEnd();
 *   // ... fixed updates ...
 *   profiler.markUpdateEnd(numUpdates);
 *   // ... render submission ...
 *   profiler.markRenderEnd(drawCalls, sprites);
 *   // ... swap buffers ...
 *   profiler.markSwapEnd();
 *   profiler.endFrame();   // commits FrameSample
 */
class FrameProfiler {
public:
    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    void beginFrame ();
    void markInputEnd();
    void markUpdateEnd(int numUpdates);
    void markRenderEnd(int drawCalls, int sprites);
    void markSwapEnd  ();
    void endFrame     ();

    // Direct sample insertion (used by benchmark mode to add completed samples)
    void addSample(const FrameSample& s);

    const std::vector<FrameSample>& samples() const { return m_samples; }
    void clearSamples() { m_samples.clear(); }

    /// Compute aggregate statistics over all stored samples.
    /// Returns a zero-filled struct if no samples are present.
    ProfilerStats computeStats() const;

    /// Write per-frame CSV with stable header.
    /// Returns false and logs if file cannot be opened.
    bool writeCsv(const std::string& path) const;

private:
    std::vector<FrameSample> m_samples;
    uint64_t  m_frameIndex      = 0;
    TimePoint m_frameStart;
    TimePoint m_inputEnd;
    TimePoint m_updateEnd;
    TimePoint m_renderEnd;
    TimePoint m_swapEnd;
    int       m_currentUpdates  = 0;
    int       m_currentDrawCalls= 0;
    int       m_currentSprites  = 0;

    static double toMs(TimePoint a, TimePoint b);
};

} // namespace rll
