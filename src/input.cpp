#include "renderlooplab/input.hpp"

#include <algorithm>
#include <cmath>

namespace rll {

void InputState::resetPerFrame()
{
    m_pressed.fill(false);
    // Note: m_held is NOT cleared here — it tracks ongoing key-down state.
    m_analogMovement = {0.0f, 0.0f};
}

void InputState::setPressed(InputAction action, bool value)
{
    m_pressed[static_cast<std::size_t>(action)] = value;
}

void InputState::setHeld(InputAction action, bool value)
{
    m_held[static_cast<std::size_t>(action)] = value;
}

bool InputState::isPressed(InputAction action) const
{
    return m_pressed[static_cast<std::size_t>(action)];
}

bool InputState::isHeld(InputAction action) const
{
    return m_held[static_cast<std::size_t>(action)];
}

void InputState::setAnalogMovement(glm::vec2 v)
{
    m_analogMovement = v;
}

glm::vec2 InputState::getDigitalMovement() const
{
    glm::vec2 dir{0.0f, 0.0f};
    if (isHeld(InputAction::MoveLeft))  dir.x -= 1.0f;
    if (isHeld(InputAction::MoveRight)) dir.x += 1.0f;
    if (isHeld(InputAction::MoveUp))    dir.y -= 1.0f;  // Y-down screen space
    if (isHeld(InputAction::MoveDown))  dir.y += 1.0f;
    return dir;
}

glm::vec2 InputState::getMovement() const
{
    glm::vec2 combined = getDigitalMovement() + m_analogMovement;

    // Normalize if magnitude exceeds 1 (prevents faster diagonal movement).
    const float len = std::sqrt(combined.x * combined.x + combined.y * combined.y);
    if (len > 1.0f) combined /= len;

    return combined;
}

} // namespace rll
