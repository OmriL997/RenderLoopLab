#pragma once

#include <chrono>

namespace rll {

/**
 * Production-inspired fixed-timestep loop controller.
 *
 * Usage:
 *   FixedTimestep ts(60.0);
 *   ts.start();
 *   while (running) {
 *       auto [nUpdates, alpha, dt] = ts.tick();
 *       for (int i = 0; i < nUpdates; ++i)
 *           simulation.update(ts.fixedDelta());
 *       renderer.render(alpha);
 *   }
 *
 * Frame-delta clamping prevents the spiral-of-death:
 *   raw delta is clamped to maxDelta before adding to the accumulator.
 *
 * A maximum of maxUpdates fixed steps are run per rendered frame.
 * Any excess accumulated time is silently discarded — this is intentional
 * and documented: the simulation lags briefly rather than spiralling.
 */
class FixedTimestep {
public:
    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    /// @param fixedHz    Simulation frequency (Hz).  Default: 60.
    /// @param maxDelta   Maximum raw frame time before clamping (seconds). Default: 0.25.
    /// @param maxUpdates Maximum fixed steps per rendered frame. Default: 5.
    explicit FixedTimestep(double fixedHz  = 60.0,
                           double maxDelta = 0.25,
                           int    maxUpdates = 5);

    /// Record the start time (call once before the loop).
    void start();

    struct TickResult {
        int    numUpdates;    ///< Fixed steps to execute this frame
        double alpha;         ///< Interpolation factor ∈ [0, 1)
        double frameTimeSec;  ///< Clamped frame delta (seconds)
    };

    /// Call once per rendered frame; advances the clock.
    TickResult tick();

    double fixedDelta() const { return m_fixedDelta; }

private:
    double     m_fixedDelta;
    double     m_maxDelta;
    int        m_maxUpdates;
    double     m_accumulator = 0.0;
    TimePoint  m_lastTime;
    bool       m_started     = false;
};

} // namespace rll
