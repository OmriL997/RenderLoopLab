#include "renderlooplab/platform/sdl_input.hpp"
#include "renderlooplab/logging.hpp"

#include <SDL.h>

#include <cmath>
#include <string>

namespace rll::platform {

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------

SdlInput::~SdlInput()
{
    if (m_controller) {
        SDL_GameControllerClose(m_controller);
        m_controller = nullptr;
    }
}

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

// SDL axis range is [−32768, 32767].  Map to [−1, 1] and apply dead-zone.
glm::vec2 SdlInput::processStick(float rawX, float rawY) const
{
    const float nx = rawX / 32767.0f;
    const float ny = rawY / 32767.0f;
    const float mag = std::sqrt(nx * nx + ny * ny);
    if (mag < m_deadZone) return {0.0f, 0.0f};

    // Scale so the dead-zone boundary maps to 0
    const float scale = (mag - m_deadZone) / (1.0f - m_deadZone);
    const float clampedScale = std::min(scale / mag, 1.0f);
    return {nx * clampedScale, ny * clampedScale};
}

void SdlInput::onControllerAdded(int deviceIndex)
{
    if (m_controller) return;  // already have one
    m_controller   = SDL_GameControllerOpen(deviceIndex);
    m_controllerId = SDL_JoystickInstanceID(
                         SDL_GameControllerGetJoystick(m_controller));
    log::info("SdlInput",
        std::string("Controller connected: ") +
        SDL_GameControllerName(m_controller));
}

void SdlInput::onControllerRemoved(int instanceId)
{
    if (!m_controller || m_controllerId != instanceId) return;
    log::info("SdlInput",
        std::string("Controller disconnected (id=") +
        std::to_string(instanceId) + ")");
    SDL_GameControllerClose(m_controller);
    m_controller   = nullptr;
    m_controllerId = -1;
}

// ---------------------------------------------------------------------------
// Event pump
// ---------------------------------------------------------------------------

bool SdlInput::processEvents(InputState& state, bool& quit)
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {

        // ── Application events ────────────────────────────────────────
        case SDL_QUIT:
            state.setPressed(InputAction::Quit, true);
            state.setHeld   (InputAction::Quit, true);
            quit = true;
            break;

        // ── Keyboard ─────────────────────────────────────────────────
        case SDL_KEYDOWN: {
            const bool repeated = ev.key.repeat != 0;
            const SDL_Keycode  key = ev.key.keysym.sym;

            // setHeld on key-down (including repeats for continuous movement)
            auto setKey = [&](InputAction action) {
                if (!repeated) state.setPressed(action, true);
                state.setHeld(action, true);
            };

            switch (key) {
                case SDLK_w: case SDLK_UP:     setKey(InputAction::MoveUp);       break;
                case SDLK_s: case SDLK_DOWN:   setKey(InputAction::MoveDown);     break;
                case SDLK_a: case SDLK_LEFT:   setKey(InputAction::MoveLeft);     break;
                case SDLK_d: case SDLK_RIGHT:  setKey(InputAction::MoveRight);    break;
                case SDLK_F5:                   setKey(InputAction::ReloadShaders);    break;
                case SDLK_F6:                   setKey(InputAction::TestBrokenShader);  break;
                case SDLK_F1:                   setKey(InputAction::ToggleVSync);       break;
                case SDLK_TAB:                  setKey(InputAction::CycleSpriteCount);  break;
                case SDLK_ESCAPE:
                    state.setPressed(InputAction::Quit, true);
                    state.setHeld   (InputAction::Quit, true);
                    quit = true;
                    break;
                default: break;
            }
            break;
        }

        case SDL_KEYUP: {
            const SDL_Keycode key = ev.key.keysym.sym;
            auto clearKey = [&](InputAction action) {
                state.setHeld(action, false);
            };
            switch (key) {
                case SDLK_w: case SDLK_UP:    clearKey(InputAction::MoveUp);    break;
                case SDLK_s: case SDLK_DOWN:  clearKey(InputAction::MoveDown);  break;
                case SDLK_a: case SDLK_LEFT:  clearKey(InputAction::MoveLeft);  break;
                case SDLK_d: case SDLK_RIGHT: clearKey(InputAction::MoveRight); break;
                default: break;
            }
            break;
        }

        // ── Controller ────────────────────────────────────────────────
        case SDL_CONTROLLERDEVICEADDED:
            onControllerAdded(ev.cdevice.which);
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
            onControllerRemoved(ev.cdevice.which);
            break;

        case SDL_CONTROLLERBUTTONDOWN: {
            if (!m_controller) break;
            switch (ev.cbutton.button) {
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    state.setPressed(InputAction::MoveUp, true);
                    state.setHeld   (InputAction::MoveUp, true);
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    state.setPressed(InputAction::MoveDown, true);
                    state.setHeld   (InputAction::MoveDown, true);
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    state.setPressed(InputAction::MoveLeft, true);
                    state.setHeld   (InputAction::MoveLeft, true);
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    state.setPressed(InputAction::MoveRight, true);
                    state.setHeld   (InputAction::MoveRight, true);
                    break;
                case SDL_CONTROLLER_BUTTON_Y:  // Y button reloads shaders
                    state.setPressed(InputAction::ReloadShaders, true);
                    break;
                default: break;
            }
            break;
        }

        case SDL_CONTROLLERBUTTONUP: {
            if (!m_controller) break;
            switch (ev.cbutton.button) {
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    state.setHeld(InputAction::MoveUp, false);   break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    state.setHeld(InputAction::MoveDown, false);  break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    state.setHeld(InputAction::MoveLeft, false);  break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    state.setHeld(InputAction::MoveRight, false); break;
                default: break;
            }
            break;
        }

        case SDL_CONTROLLERAXISMOTION: {
            if (!m_controller) break;
            if (ev.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX ||
                ev.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
            {
                // Read both axes together
                const float rawX = static_cast<float>(
                    SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_LEFTX));
                const float rawY = static_cast<float>(
                    SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_LEFTY));
                state.setAnalogMovement(processStick(rawX, rawY));
            }
            break;
        }

        default: break;
        }
    }

    return !quit;
}

} // namespace rll::platform
