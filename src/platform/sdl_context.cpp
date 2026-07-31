#include "renderlooplab/platform/sdl_context.hpp"
#include "renderlooplab/logging.hpp"
#include <string>
#include <SDL.h>

namespace rll::platform {

SdlContext::SdlContext()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0) {
        m_error = SDL_GetError();
        m_valid = false;
        log::error("SdlContext", std::string("SDL_Init failed: ") + m_error);
    } else {
        m_valid = true;
        log::info("SdlContext", "SDL2 initialised.");
    }
}

SdlContext::~SdlContext()
{
    if (m_valid) {
        SDL_Quit();
        log::info("SdlContext", "SDL2 shut down.");
    }
}

} // namespace rll::platform
