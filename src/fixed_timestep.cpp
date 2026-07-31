#include "renderlooplab/fixed_timestep.hpp"

#include <algorithm>
#include <cassert>

namespace rll {

FixedTimestep::FixedTimestep(double fixedHz, double maxDelta, int maxUpdates)
    : m_fixedDelta(1.0 / fixedHz)
    , m_maxDelta(maxDelta)
    , m_maxUpdates(maxUpdates)
{
    assert(fixedHz   >  0.0);
    assert(maxDelta  >  0.0);
    assert(maxUpdates > 0);
}

void FixedTimestep::start()
{
    m_lastTime   = Clock::now();
    m_accumulator = 0.0;
    m_started     = true;
}

FixedTimestep::TickResult FixedTimestep::tick()
{
    if (!m_started) start();

    const TimePoint now      = Clock::now();
    const double    rawDelta =
        std::chrono::duration<double>(now - m_lastTime).count();
    m_lastTime = now;

    // Clamp to prevent spiral-of-death.
    const double delta = std::min(rawDelta, m_maxDelta);
    m_accumulator += delta;

    int updates = 0;
    while (m_accumulator >= m_fixedDelta && updates < m_maxUpdates) {
        m_accumulator -= m_fixedDelta;
        ++updates;
    }

    // If the accumulator still exceeds one fixed step here, we have fallen
    // behind the real-time clock beyond what maxUpdates can recover.
    // Discard excess time so the next frame starts fresh — this is
    // intentional: the simulation lags briefly rather than spiralling.
    if (m_accumulator > m_fixedDelta) {
        m_accumulator = 0.0;
    }

    const double alpha = m_accumulator / m_fixedDelta;

    return TickResult{ updates, alpha, delta };
}

} // namespace rll
