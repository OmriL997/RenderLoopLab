#include "renderlooplab/frame_profiler.hpp"
#include "renderlooplab/logging.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <numeric>
#include <vector>

namespace rll {

// ---------------------------------------------------------------------------
// Timing helpers
// ---------------------------------------------------------------------------

double FrameProfiler::toMs(TimePoint a, TimePoint b)
{
    return std::chrono::duration<double, std::milli>(b - a).count();
}

// ---------------------------------------------------------------------------
// Per-frame recording
// ---------------------------------------------------------------------------

void FrameProfiler::beginFrame()
{
    m_frameStart      = Clock::now();
    m_currentUpdates  = 0;
    m_currentDrawCalls= 0;
    m_currentSprites  = 0;
}

void FrameProfiler::markInputEnd()
{
    m_inputEnd = Clock::now();
}

void FrameProfiler::markUpdateEnd(int numUpdates)
{
    m_updateEnd      = Clock::now();
    m_currentUpdates = numUpdates;
}

void FrameProfiler::markRenderEnd(int drawCalls, int sprites)
{
    m_renderEnd          = Clock::now();
    m_currentDrawCalls   = drawCalls;
    m_currentSprites     = sprites;
}

void FrameProfiler::markSwapEnd()
{
    m_swapEnd = Clock::now();
}

void FrameProfiler::endFrame()
{
    const TimePoint frameEnd = Clock::now();

    FrameSample s;
    s.frame      = m_frameIndex++;
    s.inputMs    = toMs(m_frameStart, m_inputEnd);
    s.updateMs   = toMs(m_inputEnd,   m_updateEnd);
    s.renderMs   = toMs(m_updateEnd,  m_renderEnd);
    s.swapMs     = toMs(m_renderEnd,  m_swapEnd);
    s.totalMs    = toMs(m_frameStart, frameEnd);
    s.numUpdates = m_currentUpdates;
    s.drawCalls  = m_currentDrawCalls;
    s.sprites    = m_currentSprites;

    m_samples.push_back(s);
}

void FrameProfiler::addSample(const FrameSample& sample)
{
    m_samples.push_back(sample);
}

// ---------------------------------------------------------------------------
// Statistics
// ---------------------------------------------------------------------------

ProfilerStats FrameProfiler::computeStats() const
{
    if (m_samples.empty()) return {};

    const int n = static_cast<int>(m_samples.size());

    // Collect total-frame times
    std::vector<double> times(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i)
        times[static_cast<std::size_t>(i)] = m_samples[static_cast<std::size_t>(i)].totalMs;

    // Sort copy for order-statistics
    std::vector<double> sorted = times;
    std::sort(sorted.begin(), sorted.end());

    // Min / Max
    const double vmin = sorted.front();
    const double vmax = sorted.back();

    // Mean
    const double sum  = std::accumulate(times.begin(), times.end(), 0.0);
    const double mean = sum / static_cast<double>(n);

    // Median (nearest-rank, lower-bound on even count)
    // For N elements, index = N/2 (integer division)
    const double median = (n % 2 == 1)
        ? sorted[static_cast<std::size_t>(n / 2)]
        : (sorted[static_cast<std::size_t>(n / 2 - 1)] + sorted[static_cast<std::size_t>(n / 2)]) * 0.5;

    // Percentile helper (nearest-rank: ceil(p/100 * n) - 1, clamped)
    auto percentile = [&](double p) -> double {
        const auto idx = static_cast<std::size_t>(
            std::max(0, static_cast<int>(std::ceil(p / 100.0 * static_cast<double>(n))) - 1));
        return sorted[std::min(idx, sorted.size() - 1)];
    };

    // Standard deviation
    double ssq = 0.0;
    for (double t : times) {
        const double d = t - mean;
        ssq += d * d;
    }
    const double stddev = std::sqrt(ssq / static_cast<double>(n));

    // Average draw calls and triangles (2 triangles per sprite quad)
    double totalDC = 0.0, totalTri = 0.0;
    for (const auto& s : m_samples) {
        totalDC  += static_cast<double>(s.drawCalls);
        totalTri += static_cast<double>(s.sprites) * 2.0;
    }

    ProfilerStats st;
    st.sampleCount  = n;
    st.min          = vmin;
    st.max          = vmax;
    st.mean         = mean;
    st.median       = median;
    st.percentile95 = percentile(95.0);
    st.percentile99 = percentile(99.0);
    st.stddev       = stddev;
    st.avgFps       = (mean > 0.0) ? (1000.0 / mean) : 0.0;
    st.avgDrawCalls = totalDC / static_cast<double>(n);
    st.avgTriangles = totalTri / static_cast<double>(n);

    return st;
}

// ---------------------------------------------------------------------------
// CSV export
// ---------------------------------------------------------------------------

bool FrameProfiler::writeCsv(const std::string& path) const
{
    std::ofstream f(path);
    if (!f.is_open()) {
        log::error("FrameProfiler", "Cannot open CSV file: " + path);
        return false;
    }

    f << "frame,input_ms,update_ms,render_ms,swap_ms,total_ms,updates,draw_calls,sprites\n";
    for (const auto& s : m_samples) {
        f << s.frame     << ','
          << s.inputMs   << ','
          << s.updateMs  << ','
          << s.renderMs  << ','
          << s.swapMs    << ','
          << s.totalMs   << ','
          << s.numUpdates<< ','
          << s.drawCalls << ','
          << s.sprites   << '\n';
    }

    if (!f) {
        log::error("FrameProfiler", "Error writing CSV: " + path);
        return false;
    }
    return true;
}

} // namespace rll
