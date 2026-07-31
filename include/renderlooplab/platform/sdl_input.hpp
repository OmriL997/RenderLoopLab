#pragma once

#include "renderlooplab/input.hpp"

#include <string>

// Forward-declare SDL types to avoid including SDL.h in this header
struct _SDL_GameController;
typedef struct _SDL_GameController SDL_GameController;

namespace rll::platform {

/**
 * SDL2 event pump → abstract InputState translator.
 *
 * Processes the SDL event queue each frame, mapping:
 *   - Keyboard events  → InputAction pressed/held
 *   - D-pad events     → InputAction pressed/held
 *   - Left-stick axis  → InputState analog movement (dead-zoned, normalised)
 *   - Controller add/remove events → logged, no crash
 *
 * SDL key codes and SDL joystick IDs are confined to this translation unit.
 * The InputState type contains only abstract actions and a movement vector.
 *
 * Dead-zone: axis values with magnitude < deadZone are treated as zero.
 * Analog axes are in SDL range [−32768, 32767]; we normalize to [−1, 1].
 */
class SdlInput {
public:
    static constexpr float DEFAULT_DEAD_ZONE = 0.15f;

    SdlInput()  = default;
    ~SdlInput();

    SdlInput(const SdlInput&)            = delete;
    SdlInput& operator=(const SdlInput&) = delete;

    /**
     * Drain the SDL event queue and update state.
     *
     * @param state  InputState to populate (resetPerFrame must be called before).
     * @param quit   Set to true if SDL_QUIT or Escape was received.
     * @return true  Normally.  false if quit was requested (caller should break the loop).
     */
    bool processEvents(InputState& state, bool& quit);

    float deadZone() const          { return m_deadZone; }
    void  setDeadZone(float dz)     { m_deadZone = dz;   }

private:
    void onControllerAdded  (int deviceIndex);
    void onControllerRemoved(int instanceId);

    glm::vec2 processStick(float rawX, float rawY) const;

    SDL_GameController* m_controller   = nullptr;
    int                 m_controllerId = -1;      ///< SDL instance ID
    float               m_deadZone     = DEFAULT_DEAD_ZONE;
};

} // namespace rll::platform
