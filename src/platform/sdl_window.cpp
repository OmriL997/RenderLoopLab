#include "renderlooplab/platform/sdl_window.hpp"
#include "renderlooplab/logging.hpp"

#include <SDL.h>
#include <glad/glad.h>

#include <string>

namespace rll::platform {

SdlWindow::~SdlWindow()
{
    if (m_context) {
        SDL_GL_DeleteContext(m_context);
        m_context = nullptr;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
}

bool SdlWindow::create(const WindowConfig& config, std::string& errorOut)
{
    // ── GL context attributes ────────────────────────────────────────────
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   24);

#if !defined(NDEBUG)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    // ── Window creation ──────────────────────────────────────────────────
    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    if (config.hidden)
        flags |= SDL_WINDOW_HIDDEN;

    m_window = SDL_CreateWindow(
        config.title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        config.width, config.height,
        flags);

    if (!m_window) {
        errorOut = std::string("SDL_CreateWindow failed: ") + SDL_GetError();
        return false;
    }

    // ── GL context ───────────────────────────────────────────────────────
    m_context = SDL_GL_CreateContext(m_window);
    if (!m_context) {
        errorOut = std::string("SDL_GL_CreateContext failed: ") + SDL_GetError();
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
        return false;
    }

    SDL_GL_MakeCurrent(m_window, m_context);

    // ── GLAD ─────────────────────────────────────────────────────────────
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
        errorOut = "gladLoadGLLoader failed: could not load OpenGL 3.3 Core functions.";
        SDL_GL_DeleteContext(m_context);
        m_context = nullptr;
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
        return false;
    }

    // ── VSync ─────────────────────────────────────────────────────────────
    setVSync(config.vsync);

    // ── Log GPU info ──────────────────────────────────────────────────────
    const char* vendor   = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    const char* version  = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    log::info("Window", std::string("GPU vendor:   ") + (vendor   ? vendor   : "?"));
    log::info("Window", std::string("GPU renderer: ") + (renderer ? renderer : "?"));
    log::info("Window", std::string("GL version:   ") + (version  ? version  : "?"));

    return true;
}

void SdlWindow::swapBuffers()
{
    SDL_GL_SwapWindow(m_window);
}

void SdlWindow::setTitle(std::string_view title)
{
    SDL_SetWindowTitle(m_window, title.data());
}

void SdlWindow::getDrawableSize(int& w, int& h) const
{
    SDL_GL_GetDrawableSize(m_window, &w, &h);
}

void SdlWindow::getWindowSize(int& w, int& h) const
{
    SDL_GetWindowSize(m_window, &w, &h);
}

bool SdlWindow::setVSync(bool enable)
{
    if (enable) {
        // Try adaptive VSync first (-1), fall back to standard (1).
        if (SDL_GL_SetSwapInterval(-1) == 0) {
            log::info("Window", "Adaptive VSync enabled.");
            return true;
        }
        log::warn("Window", "Adaptive VSync unavailable; using standard VSync.");
        if (SDL_GL_SetSwapInterval(1) == 0)
            return true;
        log::warn("Window", "VSync could not be enabled.");
        return false;
    } else {
        if (SDL_GL_SetSwapInterval(0) == 0) {
            log::info("Window", "VSync disabled.");
            return true;
        }
        log::warn("Window", "VSync disable not supported by driver.");
        return false;
    }
}

} // namespace rll::platform
