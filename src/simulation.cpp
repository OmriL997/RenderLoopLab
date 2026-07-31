#include "renderlooplab/simulation.hpp"
#include "renderlooplab/input.hpp"

#include <algorithm>
#include <cmath>
#include <random>

namespace rll {

Simulation::Simulation(SimulationConfig config)
    : m_config(std::move(config))
{
    initSprites();
}

void Simulation::initSprites()
{
    m_sprites.clear();
    m_sprites.reserve(static_cast<std::size_t>(m_config.numSprites));

    // Seeded RNG — must produce the same sequence for the same seed.
    std::mt19937_64 rng(m_config.seed);

    const float W = m_config.worldBounds.x;
    const float H = m_config.worldBounds.y;

    // ── Player (index 0) ────────────────────────────────────────────────
    {
        SpriteState p;
        p.isPlayer        = true;
        p.size            = {40.0f, 40.0f};
        p.currentPosition = {W * 0.5f, H * 0.5f};
        p.previousPosition= p.currentPosition;
        p.velocity        = {0.0f, 0.0f};
        p.color           = {0.2f, 0.8f, 1.0f, 1.0f};  // cyan-ish
        m_sprites.push_back(p);
    }

    // ── Background sprites ───────────────────────────────────────────────
    std::uniform_real_distribution<float> posX(0.0f, W);
    std::uniform_real_distribution<float> posY(0.0f, H);
    std::uniform_real_distribution<float> velDir(-1.0f, 1.0f);
    std::uniform_real_distribution<float> speedScale(0.5f, 1.5f);
    std::uniform_real_distribution<float> sizeScale(16.0f, 48.0f);
    std::uniform_real_distribution<float> hue(0.0f, 1.0f);
    std::uniform_real_distribution<float> angVel(-1.5f, 1.5f);

    for (int i = 1; i < m_config.numSprites; ++i) {
        SpriteState s;
        s.isPlayer         = false;
        const float sz     = sizeScale(rng);
        s.size             = {sz, sz};
        s.currentPosition  = {posX(rng), posY(rng)};
        s.previousPosition = s.currentPosition;
        const float speed  = m_config.spriteSpeed * speedScale(rng);
        float vx = velDir(rng);
        float vy = velDir(rng);
        const float mag    = std::sqrt(vx * vx + vy * vy);
        if (mag > 1e-6f) { vx /= mag; vy /= mag; }
        s.velocity         = {vx * speed, vy * speed};
        s.angularVelocity  = angVel(rng);

        // Vary color with hue
        const float h6 = hue(rng) * 6.0f;
        const int   hi = static_cast<int>(h6) % 6;
        const float f  = h6 - static_cast<float>(static_cast<int>(h6));
        const float q  = 1.0f - f;
        switch (hi) {
            case 0: s.color = {1.0f, f,    0.0f, 0.9f}; break;
            case 1: s.color = {q,    1.0f, 0.0f, 0.9f}; break;
            case 2: s.color = {0.0f, 1.0f, f,    0.9f}; break;
            case 3: s.color = {0.0f, q,    1.0f, 0.9f}; break;
            case 4: s.color = {f,    0.0f, 1.0f, 0.9f}; break;
            default:s.color = {1.0f, 0.0f, q,    0.9f}; break;
        }
        m_sprites.push_back(s);
    }
}

// ---------------------------------------------------------------------------

void Simulation::update(double dt, const InputState& input)
{
    for (auto& sprite : m_sprites) {
        sprite.previousPosition = sprite.currentPosition;

        if (sprite.isPlayer)
            updatePlayer(sprite, dt, input);
        else
            updateBackground(sprite, dt);
    }
}

void Simulation::updatePlayer(SpriteState& s, double dt, const InputState& input)
{
    const glm::vec2 move = input.getMovement();  // normalised, [-1,1]
    const float     spd  = m_config.playerSpeed;
    s.velocity           = move * spd;
    s.currentPosition   += s.velocity * static_cast<float>(dt);
    clampToWorld(s);
}

void Simulation::updateBackground(SpriteState& s, double dt)
{
    s.currentPosition += s.velocity * static_cast<float>(dt);
    s.rotation        += s.angularVelocity * static_cast<float>(dt);
    bounce(s);
}

void Simulation::bounce(SpriteState& s)
{
    const float hw = s.size.x * 0.5f;
    const float hh = s.size.y * 0.5f;
    const float W  = m_config.worldBounds.x;
    const float H  = m_config.worldBounds.y;

    if (s.currentPosition.x - hw < 0.0f) {
        s.currentPosition.x = hw;
        s.velocity.x        = std::abs(s.velocity.x);
    } else if (s.currentPosition.x + hw > W) {
        s.currentPosition.x = W - hw;
        s.velocity.x        = -std::abs(s.velocity.x);
    }

    if (s.currentPosition.y - hh < 0.0f) {
        s.currentPosition.y = hh;
        s.velocity.y        = std::abs(s.velocity.y);
    } else if (s.currentPosition.y + hh > H) {
        s.currentPosition.y = H - hh;
        s.velocity.y        = -std::abs(s.velocity.y);
    }
}

void Simulation::clampToWorld(SpriteState& s)
{
    const float hw = s.size.x * 0.5f;
    const float hh = s.size.y * 0.5f;
    const float W  = m_config.worldBounds.x;
    const float H  = m_config.worldBounds.y;

    s.currentPosition.x = std::max(hw, std::min(W - hw, s.currentPosition.x));
    s.currentPosition.y = std::max(hh, std::min(H - hh, s.currentPosition.y));
}

} // namespace rll
