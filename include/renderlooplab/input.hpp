#pragma once

#include <array>
#include <cstddef>

#include <glm/glm.hpp>

namespace rll {

/// Abstract input actions. SDL key/controller codes must not appear outside
/// the platform::SdlInput layer.
enum class InputAction {
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    ReloadShaders,
    ToggleVSync,
    Quit,

    Count  ///< Sentinel — keep last
};

constexpr std::size_t INPUT_ACTION_COUNT =
    static_cast<std::size_t>(InputAction::Count);

/**
 * Per-frame snapshot of abstract input state.
 *
 * Lifecycle:
 *   1. Call resetPerFrame() at the start of each frame.
 *   2. The SDL input layer calls setPressed/setHeld as it processes events.
 *   3. Simulation and application code query isPressed/isHeld/getMovement.
 *
 * isPressed() – true only for the single frame the key was first pressed.
 * isHeld()    – true for every frame the key is physically held.
 *
 * Analog movement from the controller is normalized and dead-zone-filtered
 * before being stored; see SdlInput for details.
 */
class InputState {
public:
    InputState() { resetPerFrame(); }

    // ── Per-frame reset ────────────────────────────────────────────────
    /// Clears all "pressed this frame" flags. Does NOT clear held flags.
    void resetPerFrame();

    // ── Setters (used by the SDL input layer) ─────────────────────────
    void setPressed(InputAction action, bool value);
    void setHeld   (InputAction action, bool value);
    void setAnalogMovement(glm::vec2 v);

    // ── Queries ───────────────────────────────────────────────────────
    bool isPressed(InputAction action) const;
    bool isHeld   (InputAction action) const;

    /// Digital movement vector derived from keyboard/d-pad held state.
    /// Components are −1, 0, or +1; diagonal is NOT yet normalised here.
    glm::vec2 getDigitalMovement() const;

    /// Analog movement from the left stick (already normalized, dead-zoned).
    glm::vec2 getAnalogMovement() const { return m_analogMovement; }

    /// Combined and normalised movement (digital + analog, clamped to unit).
    glm::vec2 getMovement() const;

private:
    std::array<bool, INPUT_ACTION_COUNT> m_pressed{};
    std::array<bool, INPUT_ACTION_COUNT> m_held{};
    glm::vec2 m_analogMovement{0.0f, 0.0f};
};

} // namespace rll
