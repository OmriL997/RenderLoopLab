#pragma once

#include <string>
#include <string_view>

struct SDL_Window;

namespace rll::platform {

struct WindowConfig {
    std::string title  = "RenderLoopLab";
    int         width  = 1280;
    int         height = 720;
    bool        hidden = false;
    bool        vsync  = true;
};

/**
 * Owns an SDL_Window and its associated OpenGL context (SDL_GLContext).
 *
 * Lifetime requirement:
 *   The SdlWindow must outlive all OpenGL resource wrappers.
 *   When ~SdlWindow() runs it calls SDL_GL_DeleteContext then SDL_DestroyWindow.
 *   Any remaining glad_gl* function pointers become invalid afterwards.
 *
 * OpenGL 3.3 Core context attributes are requested via SDL_GL_SetAttribute
 * before window creation.
 *
 * GLAD must be initialised (gladLoadGLLoader) after create() succeeds.
 */
class SdlWindow {
public:
    SdlWindow() = default;
    ~SdlWindow();

    SdlWindow(const SdlWindow&)            = delete;
    SdlWindow& operator=(const SdlWindow&) = delete;
    SdlWindow(SdlWindow&&)                 = delete;
    SdlWindow& operator=(SdlWindow&&)      = delete;

    /// Create the window and GL context.  Returns true on success.
    bool create(const WindowConfig& config, std::string& errorOut);

    void swapBuffers();
    void setTitle(std::string_view title);

    /// Returns the drawable (framebuffer) size — use this for glViewport.
    void getDrawableSize(int& w, int& h) const;

    /// Returns the logical window size (may differ on HiDPI displays).
    void getWindowSize(int& w, int& h) const;

    /// Attempt to enable/disable VSync.
    /// Returns true if the requested state was applied.
    /// Logs a warning if adaptive VSync is unavailable but standard succeeds.
    bool setVSync(bool enable);

    bool        valid    () const { return m_window != nullptr; }
    SDL_Window* rawWindow() const { return m_window;            }

private:
    SDL_Window* m_window  = nullptr;
    void*       m_context = nullptr;  ///< SDL_GLContext (opaque void*)
};

} // namespace rll::platform
