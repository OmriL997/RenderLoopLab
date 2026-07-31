#include "renderlooplab/application.hpp"
#include "renderlooplab/application_config.hpp"
#include "renderlooplab/benchmark_report.hpp"
#include "renderlooplab/fixed_timestep.hpp"
#include "renderlooplab/frame_profiler.hpp"
#include "renderlooplab/input.hpp"
#include "renderlooplab/logging.hpp"
#include "renderlooplab/simulation.hpp"
#include "renderlooplab/version.hpp"

#include "renderlooplab/graphics/renderer_2d.hpp"
#include "renderlooplab/graphics/shader_program.hpp"
#include "renderlooplab/graphics/texture_2d.hpp"

#include "renderlooplab/platform/sdl_context.hpp"
#include "renderlooplab/platform/sdl_input.hpp"
#include "renderlooplab/platform/sdl_window.hpp"

#include <glad/glad.h>
#include <SDL.h>

#include <filesystem>
#include <string>
#include <chrono>
#include <cstdio>

namespace rll {

namespace fs = std::filesystem;

// ---------------------------------------------------------------------------
// Asset resolution
// ---------------------------------------------------------------------------

namespace {

fs::path resolveAssetRoot(const std::string& override)
{
    if (!override.empty()) {
        const fs::path p(override);
        if (fs::exists(p)) return p;
        log::warn("Application", "Provided --asset-root does not exist: " + override);
    }

    // Try next to the executable using SDL_GetBasePath()
    // SDL is available here, but to keep things simple we use argv[0] fallback
    // and the documented "adjacent to executable" resolution.
    // SDL_GetBasePath is called inside Application::Impl so it's fine.
    return {};
}

} // anon

// ---------------------------------------------------------------------------
// PIMPL
// ---------------------------------------------------------------------------

class Application::Impl {
public:
    explicit Impl(ApplicationConfig cfg) : m_cfg(std::move(cfg)) {}

    int run();

private:
    int  runInteractive();
    int  runBenchmark();
    int  runSmokeTest();

    bool init(std::string& err);
    void shutdown();

    fs::path shaderPath(const std::string& filename) const;
    bool     loadShaders(bool isReload);

    void handleResize(int w, int h);
    void updateWindowTitle(double fps, double frameMs, int sprites);

    ApplicationConfig              m_cfg;

    // Platform (lifetime: context > window > glad > graphics)
    platform::SdlContext           m_sdlCtx;
    platform::SdlWindow            m_window;
    platform::SdlInput             m_input;

    // Graphics
    gfx::ShaderProgram             m_shader;
    gfx::Texture2D                 m_texture;
    gfx::Renderer2D                m_renderer;

    // Simulation
    InputState                     m_inputState;
    std::unique_ptr<Simulation>    m_sim = nullptr;

    // Profiling
    FrameProfiler                  m_profiler;

    // Runtime state
    fs::path                       m_assetRoot;
    int                            m_drawableW = 0;
    int                            m_drawableH = 0;
    bool                           m_vsync     = true;
};

// ---------------------------------------------------------------------------
// Application public API (delegates to Impl)
// ---------------------------------------------------------------------------

Application::Application(ApplicationConfig config)
    : m_impl(std::make_unique<Impl>(std::move(config)))
{}

Application::~Application() = default;

int Application::run()
{
    return m_impl->run();
}

// ---------------------------------------------------------------------------
// Impl: run()
// ---------------------------------------------------------------------------

int Application::Impl::run()
{
    if (m_cfg.benchmark) return runBenchmark();
    if (m_cfg.smokeTest) return runSmokeTest();
    return runInteractive();
}

// ---------------------------------------------------------------------------
// Impl: init()
// ---------------------------------------------------------------------------

bool Application::Impl::init(std::string& err)
{
    // SDL
    if (!m_sdlCtx.valid()) {
        err = std::string("SDL init failed: ") + m_sdlCtx.error();
        return false;
    }

    // Window + GL context
    platform::WindowConfig wc;
    wc.title  = std::string("RenderLoopLab v") + VERSION_STRING;
    wc.width  = m_cfg.width;
    wc.height = m_cfg.height;
    wc.hidden = m_cfg.hidden;
    wc.vsync  = m_cfg.vsync;
    m_vsync   = m_cfg.vsync;

    if (!m_window.create(wc, err)) return false;

    m_window.getDrawableSize(m_drawableW, m_drawableH);

    // GL state
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, m_drawableW, m_drawableH);

    // Asset root
    {
        // SDL_GetBasePath returns the directory that contains the executable
        char* basePath = SDL_GetBasePath();
        if (basePath) {
            const fs::path adjacent = fs::path(basePath) / "assets";
            SDL_free(basePath);
            if (!m_cfg.assetRoot.empty()) {
                m_assetRoot = m_cfg.assetRoot;
            } else if (fs::exists(adjacent)) {
                m_assetRoot = adjacent;
            } else {
                // Development fallback: look for assets/ relative to CWD
                const fs::path cwd = fs::current_path() / "assets";
                if (fs::exists(cwd))
                    m_assetRoot = cwd;
                else
                    m_assetRoot = adjacent; // will fail gracefully at shader load
            }
        }
    }

    // Shaders
    if (!loadShaders(false)) {
        err = "Shader initialisation failed.";
        return false;
    }

    // Texture
    m_texture = gfx::Texture2D::createCheckerboard(4, 8);

    // Renderer
    if (!m_renderer.init(m_shader, m_texture)) {
        err = "Renderer2D initialisation failed.";
        return false;
    }
    m_renderer.setProjection(m_drawableW, m_drawableH);

    // Simulation
    SimulationConfig simCfg;
    simCfg.numSprites  = m_cfg.sprites;
    simCfg.seed        = m_cfg.seed;
    simCfg.worldBounds = {static_cast<float>(m_cfg.width),
                          static_cast<float>(m_cfg.height)};
    m_sim = std::make_unique<Simulation>(simCfg);

    log::info("Application", "Initialisation complete.");
    return true;
}

void Application::Impl::shutdown()
{
    m_sim.reset();
    log::info("Application", "Shutdown complete.");
}

// ---------------------------------------------------------------------------
// Shader loading / hot-reload
// ---------------------------------------------------------------------------

fs::path Application::Impl::shaderPath(const std::string& filename) const
{
    return m_assetRoot / "shaders" / filename;
}

bool Application::Impl::loadShaders(bool isReload)
{
    const std::string vertPath = shaderPath("sprite.vert").string();
    const std::string fragPath = shaderPath("sprite.frag").string();
    const char* tag = isReload ? "ShaderReload" : "ShaderProgram";

    gfx::ShaderProgram newShader;
    std::string err;

    if (!gfx::ShaderProgram::createFromFiles(vertPath, fragPath, newShader, err)) {
        log::error(tag, err);
        if (isReload) {
            log::error(tag, "Shader reload failed. Previous shader program remains active.");
        }
        return false;
    }

    m_shader = std::move(newShader);

    if (isReload) {
        log::info(tag, "Shader reload succeeded: sprite.vert + sprite.frag");
        // Renderer holds a pointer to m_shader — it is still the same address,
        // but we must re-set the projection uniform via the new program.
        m_renderer.setProjection(m_drawableW, m_drawableH);
    }
    return true;
}

// ---------------------------------------------------------------------------
// Resize
// ---------------------------------------------------------------------------

void Application::Impl::handleResize(int w, int h)
{
    if (w == m_drawableW && h == m_drawableH) return;
    m_drawableW = w;
    m_drawableH = h;
    glViewport(0, 0, w, h);
    m_renderer.setProjection(w, h);

    if (m_sim) {
        // Keep world bounds in sync with drawable size
        SimulationConfig sc = m_sim->config();
        sc.worldBounds = {static_cast<float>(w), static_cast<float>(h)};
        m_sim.reset();
        m_sim = std::make_unique<Simulation>(sc);
    }
}

// ---------------------------------------------------------------------------
// Window title
// ---------------------------------------------------------------------------

void Application::Impl::updateWindowTitle(double fps, double frameMs, int sprites)
{
    char buf[256];
    std::snprintf(buf, sizeof(buf),
        "RenderLoopLab | %.1f FPS | %.2f ms | Sprites: %d",
        fps, frameMs, sprites);
    m_window.setTitle(buf);
}

// ---------------------------------------------------------------------------
// Interactive run
// ---------------------------------------------------------------------------

int Application::Impl::runInteractive()
{
    std::string err;
    if (!init(err)) {
        log::error("Application", err);
        shutdown();
        return 2;
    }

    FixedTimestep ts(m_cfg.fixedHz);
    ts.start();

    using Clock = std::chrono::steady_clock;
    auto titleTimer = Clock::now();
    double recentFps  = 0.0;
    double recentMs   = 0.0;

    bool running = true;

    while (running) {
        m_profiler.beginFrame();

        // ── Events & input ────────────────────────────────────────────
        m_inputState.resetPerFrame();
        bool quit = false;
        m_input.processEvents(m_inputState, quit);
        if (quit) running = false;

        // Handle special one-shot actions
        if (m_inputState.isPressed(InputAction::ReloadShaders)) {
            loadShaders(true);
        }
        if (m_inputState.isPressed(InputAction::ToggleVSync)) {
            m_vsync = !m_vsync;
            m_window.setVSync(m_vsync);
        }

        m_profiler.markInputEnd();

        // ── Fixed simulation ──────────────────────────────────────────
        const auto tick = ts.tick();
        for (int i = 0; i < tick.numUpdates; ++i)
            m_sim->update(ts.fixedDelta(), m_inputState);

        m_profiler.markUpdateEnd(tick.numUpdates);

        // ── Resize check ──────────────────────────────────────────────
        {
            int dw, dh;
            m_window.getDrawableSize(dw, dh);
            handleResize(dw, dh);
        }

        // ── Render ────────────────────────────────────────────────────
        glClearColor(0.10f, 0.10f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_renderer.beginFrame();
        for (const auto& s : m_sim->sprites()) {
            // Interpolate between previous and current position
            const float alpha = static_cast<float>(tick.alpha);
            const glm::vec2 pos =
                s.previousPosition * (1.0f - alpha) + s.currentPosition * alpha;

            gfx::SpriteDrawCmd cmd;
            cmd.position = pos;
            cmd.size     = s.size;
            cmd.rotation = s.rotation;
            cmd.color    = s.color;
            m_renderer.drawSprite(cmd);
        }
        const auto stats = m_renderer.endFrame();

        m_profiler.markRenderEnd(stats.drawCalls, stats.sprites);

        // ── Swap ──────────────────────────────────────────────────────
        m_window.swapBuffers();
        m_profiler.markSwapEnd();
        m_profiler.endFrame();

        // ── Window title (update ~4× per second) ──────────────────────
        const auto now = Clock::now();
        if (std::chrono::duration<double>(now - titleTimer).count() >= 0.25) {
            titleTimer = now;
            if (!m_profiler.samples().empty()) {
                const auto& last = m_profiler.samples().back();
                recentMs  = last.totalMs;
                recentFps = recentMs > 0.0 ? 1000.0 / recentMs : 0.0;
            }
            updateWindowTitle(recentFps, recentMs, stats.sprites);
        }
    }

    shutdown();
    return 0;
}

// ---------------------------------------------------------------------------
// Benchmark run
// ---------------------------------------------------------------------------

int Application::Impl::runBenchmark()
{
    std::string err;
    if (!init(err)) {
        log::error("Application", err);
        shutdown();
        return 2;
    }

    log::info("Benchmark", "Starting warm-up (" +
              std::to_string(m_cfg.warmupFrames) + " frames)...");

    FixedTimestep ts(m_cfg.fixedHz);
    ts.start();

    // ── Warm-up ────────────────────────────────────────────────────────
    for (int f = 0; f < m_cfg.warmupFrames; ++f) {
        m_inputState.resetPerFrame();
        bool quit = false;
        m_input.processEvents(m_inputState, quit);
        if (quit) { shutdown(); return 0; }

        const auto tick = ts.tick();
        for (int i = 0; i < tick.numUpdates; ++i)
            m_sim->update(ts.fixedDelta(), m_inputState);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        m_renderer.beginFrame();
        for (const auto& s : m_sim->sprites()) {
            const float a = static_cast<float>(tick.alpha);
            gfx::SpriteDrawCmd cmd;
            cmd.position = s.previousPosition * (1.0f - a) + s.currentPosition * a;
            cmd.size     = s.size;
            cmd.rotation = s.rotation;
            cmd.color    = s.color;
            m_renderer.drawSprite(cmd);
        }
        m_renderer.endFrame();
        m_window.swapBuffers();
    }

    log::info("Benchmark", "Measuring " +
              std::to_string(m_cfg.measuredFrames) + " frames...");

    m_profiler.clearSamples();

    // ── Measurement ────────────────────────────────────────────────────
    for (int f = 0; f < m_cfg.measuredFrames; ++f) {
        m_profiler.beginFrame();

        m_inputState.resetPerFrame();
        bool quit = false;
        m_input.processEvents(m_inputState, quit);
        if (quit) break;
        m_profiler.markInputEnd();

        const auto tick = ts.tick();
        for (int i = 0; i < tick.numUpdates; ++i)
            m_sim->update(ts.fixedDelta(), m_inputState);
        m_profiler.markUpdateEnd(tick.numUpdates);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        m_renderer.beginFrame();
        for (const auto& s : m_sim->sprites()) {
            const float a = static_cast<float>(tick.alpha);
            gfx::SpriteDrawCmd cmd;
            cmd.position = s.previousPosition * (1.0f - a) + s.currentPosition * a;
            cmd.size     = s.size;
            cmd.rotation = s.rotation;
            cmd.color    = s.color;
            m_renderer.drawSprite(cmd);
        }
        const auto stats = m_renderer.endFrame();
        m_profiler.markRenderEnd(stats.drawCalls, stats.sprites);

        m_window.swapBuffers();
        m_profiler.markSwapEnd();
        m_profiler.endFrame();
    }

    // ── Report ────────────────────────────────────────────────────────
    BenchmarkReport report;
    report.env.operatingSystem = osIdentString();
    report.env.compiler        = compilerIdentString();
    {
        const char* v = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        const char* r = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        const char* o = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        report.env.gpuVendor    = v ? v : "";
        report.env.gpuRenderer  = r ? r : "";
        report.env.openglVersion= o ? o : "";
    }
    report.config = m_cfg;
    report.stats  = m_profiler.computeStats();

    printBenchmarkSummary(report);

    int exitCode = 0;

    if (!m_cfg.csvPath.empty()) {
        if (!m_profiler.writeCsv(m_cfg.csvPath)) {
            log::error("Benchmark", "CSV write failed: " + m_cfg.csvPath);
            exitCode = 3;
        } else {
            log::info("Benchmark", "CSV written: " + m_cfg.csvPath);
        }
    }
    if (!m_cfg.reportPath.empty()) {
        if (!writeBenchmarkJson(m_cfg.reportPath, report)) {
            log::error("Benchmark", "JSON write failed: " + m_cfg.reportPath);
            exitCode = 3;
        } else {
            log::info("Benchmark", "JSON report written: " + m_cfg.reportPath);
        }
    }

    shutdown();
    return exitCode;
}

// ---------------------------------------------------------------------------
// Smoke-test run
// ---------------------------------------------------------------------------

int Application::Impl::runSmokeTest()
{
    std::string err;
    if (!init(err)) {
        log::error("SmokeTest", err);
        shutdown();
        return 2;
    }

    log::info("SmokeTest", "Running " +
              std::to_string(m_cfg.measuredFrames) + " frames...");

    FixedTimestep ts(m_cfg.fixedHz);
    ts.start();

    for (int f = 0; f < m_cfg.measuredFrames; ++f) {
        m_inputState.resetPerFrame();
        bool quit = false;
        m_input.processEvents(m_inputState, quit);
        if (quit) break;

        const auto tick = ts.tick();
        for (int i = 0; i < tick.numUpdates; ++i)
            m_sim->update(ts.fixedDelta(), m_inputState);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        m_renderer.beginFrame();
        for (const auto& s : m_sim->sprites()) {
            const float a = static_cast<float>(tick.alpha);
            gfx::SpriteDrawCmd cmd;
            cmd.position = s.previousPosition * (1.0f - a) + s.currentPosition * a;
            cmd.size     = s.size;
            cmd.rotation = s.rotation;
            cmd.color    = s.color;
            m_renderer.drawSprite(cmd);
        }
        m_renderer.endFrame();
        m_window.swapBuffers();

        // Basic sanity: check for GL errors
        const GLenum glErr = glGetError();
        if (glErr != GL_NO_ERROR) {
            log::error("SmokeTest",
                "OpenGL error 0x" + std::to_string(glErr) +
                " on frame " + std::to_string(f));
            shutdown();
            return 4;
        }
    }

    log::info("SmokeTest", "Smoke test passed.");
    shutdown();
    return 0;
}

} // namespace rll
