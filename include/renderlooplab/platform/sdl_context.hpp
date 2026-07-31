#pragma once

namespace rll::platform {

/**
 * RAII wrapper for SDL_Init / SDL_Quit.
 *
 * Initialises SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS.
 * Exactly one SdlContext should exist for the duration of the application.
 *
 * Check valid() after construction before proceeding.
 */
class SdlContext {
public:
    SdlContext();
    ~SdlContext();

    SdlContext(const SdlContext&)            = delete;
    SdlContext& operator=(const SdlContext&) = delete;
    SdlContext(SdlContext&&)                 = delete;
    SdlContext& operator=(SdlContext&&)      = delete;

    bool        valid() const { return m_valid; }
    const char* error() const { return m_error; }

private:
    bool        m_valid = false;
    const char* m_error = nullptr;
};

} // namespace rll::platform
