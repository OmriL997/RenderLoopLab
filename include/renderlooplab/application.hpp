#pragma once

#include "renderlooplab/application_config.hpp"

#include <memory>

namespace rll {

/**
 * Top-level application orchestrator.
 *
 * Owns and coordinates:
 *   SdlContext → SdlWindow (+ GL context) → GLAD → Shaders
 *   → Renderer2D → Simulation → FixedTimestep → FrameProfiler
 *
 * The main game loop runs inside run(). Returns an exit code:
 *   0 – success
 *   2 – SDL / OpenGL / shader initialisation failure
 *   3 – benchmark CSV or JSON write failure
 *   4 – smoke-test failure
 *
 * The PIMPL idiom keeps SDL and OpenGL types out of this header,
 * making it includable by CPU-only code without platform headers.
 */
class Application {
public:
    explicit Application(ApplicationConfig config);
    ~Application();

    Application(const Application&)            = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&)                 = delete;
    Application& operator=(Application&&)      = delete;

    int run();

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace rll
