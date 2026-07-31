#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace rll {

class InputState;

/// State of a single sprite, including both the previous and current
/// simulation positions to allow smooth rendering interpolation.
struct SpriteState {
    glm::vec2 previousPosition{0.0f, 0.0f};
    glm::vec2 currentPosition {0.0f, 0.0f};
    glm::vec2 velocity        {0.0f, 0.0f};
    glm::vec2 size            {32.0f, 32.0f};
    float     rotation        {0.0f};    ///< Current rotation in radians
    float     angularVelocity {0.0f};    ///< Radians per second
    glm::vec4 color           {1.0f, 1.0f, 1.0f, 1.0f};
    bool      isPlayer        {false};
};

struct SimulationConfig {
    int       numSprites   = 500;
    uint64_t  seed         = 12345;
    glm::vec2 worldBounds  = {1280.0f, 720.0f};  ///< Width × Height in world units
    float     playerSpeed  = 300.0f;              ///< Pixels per second
    float     spriteSpeed  = 80.0f;               ///< Background sprite base speed
};

/**
 * Deterministic 2D simulation.
 *
 * Rules:
 *  - sprites()[0] is the player.
 *  - Background sprites bounce off world-boundary walls.
 *  - The player is clamped to world-boundary edges.
 *  - All randomness is seeded by SimulationConfig::seed.
 *  - update() is pure CPU; no OpenGL or SDL headers included.
 *  - Identical config + seed + update sequence = identical state.
 */
class Simulation {
public:
    explicit Simulation(SimulationConfig config);

    /// Advance by one fixed time step.  dt must equal fixedDelta seconds.
    void update(double dt, const InputState& input);

    const std::vector<SpriteState>& sprites() const { return m_sprites; }
    const SimulationConfig&         config()  const { return m_config;  }

private:
    void initSprites();
    void updatePlayer    (SpriteState& s, double dt, const InputState& input);
    void updateBackground(SpriteState& s, double dt);
    void bounce          (SpriteState& s);
    void clampToWorld    (SpriteState& s);

    SimulationConfig          m_config;
    std::vector<SpriteState>  m_sprites;  ///< [0] = player
};

} // namespace rll
