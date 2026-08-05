# RenderLoopLab

[![CI](https://github.com/OmriL997/RenderLoopLab/actions/workflows/ci.yml/badge.svg)](https://github.com/OmriL997/RenderLoopLab/actions/workflows/ci.yml)

**RenderLoopLab** is a C++17 real-time 2D rendering laboratory built with SDL2 and OpenGL 3.3. It demonstrates a fixed-timestep simulation, variable-rate rendering, keyboard and controller input, move-only RAII wrappers for GPU resources, shader hot reloading, and frame-time benchmarking.

**C++17、SDL2、OpenGL 3.3で実装したリアルタイム2Dレンダリング実験環境です。固定タイムステップ、可変レート描画、キーボード／ゲームパッド入力、GPUリソースのRAII管理、シェーダーのホットリロード、フレーム時間計測およびベンチマーク出力を備えています。**

---

## Overview

RenderLoopLab is a portfolio-scale project designed to demonstrate practical knowledge of real-time application structure, game loops, GPU resource ownership, and automated testing in C++17. It is a focused runtime laboratory and not a general-purpose game engine.

### Features

- **Fixed-timestep simulation** with configurable frequency (default 60 Hz)
- **Variable-rate rendering** with position interpolation for smooth motion
- **Dynamic-batch 2D renderer** - all sprites drawn in a single `glDrawElements` call
- **Move-only RAII wrappers** for all OpenGL resources (VAO, VBO, IBO, shader, texture)
- **Keyboard and game-controller input** via SDL2
- **Shader hot reloading** (F5) - failed reloads retain the previous working program
- **Procedural checkerboard texture** - no external image files required
- **Frame-time profiler** with CSV export
- **Benchmark mode** with JSON summary report
- **CI smoke-test mode** for headless environments
- **GitHub Actions CI** for Ubuntu (GCC) and Windows (MSVC)
- **Catch2 v3 unit tests** covering simulation, input, profiler, CLI, and resource ownership

---

## Controls

| Key / Input | Action |
|---|---|
| **W / Up Arrow** | Move player up |
| **S / Down Arrow** | Move player down |
| **A / Left Arrow** | Move player left |
| **D / Right Arrow** | Move player right |
| **Left analog stick** | Move player (game controller) |
| **D-pad** | Move player (game controller) |
| **F5** | Toggle shaders (default ↔ alt grayscale) |
| **F6** | Attempt broken shader compile (fallback demo) |
| **Tab** | Cycle sprite count (20 → 100 → 500 → 1000 → 5000) |
| **F1** | Toggle VSync |
| **Escape** | Quit |
| **Y button (controller)** | Reload shaders |

---

## Demo

### Shader Hot-Reload (F5)

Press **F5** to toggle between two live fragment shaders — no restart required:

| State | Window title | Visual |
|---|---|---|
| Before | `RenderLoopLab \| default \| 60.0 FPS \| …` | Full-colour sprites |
| After  | `RenderLoopLab \| alt (grayscale) \| 60.0 FPS \| …` | Grayscale sprites |

Press **F5** again to switch back. The shaders are recompiled from disk each time.

### Shader Compile Failure & Fallback (F6)

Press **F6** to deliberately compile a broken shader (`broken_sprite.frag.example`).
The GLSL error is printed to the console, rendering continues uninterrupted,
and the window title briefly shows `[err: compile failed]`:

```
[ERROR] [ShaderReload] Testing broken shader compile (fallback demo)...
[ERROR] [ShaderReload] Fragment shader compilation failed (broken_sprite.frag.example):
        ERROR: 0:19: '' : syntax error: #version
[INFO]  [ShaderReload] Compile failed as expected — previous shader remains active.
```

### Benchmark Output

```
RenderLoopLab Benchmark
-----------------------
Resolution:          1280x720
Sprites:             500
Warmup frames:       60
Measured frames:     300
Seed:                12345
VSync:               off

Average frame time:  16.49 ms
Average FPS:         60.64
Minimum:             16.31 ms
Maximum:             17.14 ms
Median:              16.49 ms
95th percentile:     16.73 ms
99th percentile:     17.01 ms
Std deviation:       0.11 ms
Avg draw calls/frame: 1.00
Avg triangles/frame:  1000.00
```

### CSV Output (`demo/example_benchmark.csv`)

```
frame,input_ms,update_ms,render_ms,swap_ms,total_ms,updates,draw_calls,sprites
0,0.04,0.31,0.52,15.62,16.49,1,1,500
1,0.03,0.29,0.51,15.68,16.51,1,1,500
2,0.03,0.30,0.53,15.61,16.47,1,1,500
...
```

### JSON Report (`demo/example_benchmark.json`)

```json
{
  "tool": "RenderLoopLab",
  "environment": { "operating_system": "Windows 11", "compiler": "MSVC 19.43",
                   "gpu_renderer": "NVIDIA GeForce RTX 4060 Ti/PCIe/SSE2" },
  "timing_ms": { "mean": 16.49, "percentile_95": 16.73, "percentile_99": 17.01 },
  "average_fps": 60.64
}
```

Full example files are in [`demo/`](demo/).

### Test Count

```
69 test cases / 336 assertions — all pass
```

Run with: `ctest --test-dir build -C Release --output-on-failure`

### CI Environments

| Job | Platform | What runs |
|---|---|---|
| `ubuntu-gcc` | Ubuntu (GCC, Release) | Build → tests → smoke test → benchmark → artifact upload |
| `ubuntu-sanitizers` | Ubuntu (GCC, ASan+UBSan) | CPU tests under AddressSanitizer + UBSan |
| `windows-msvc` | Windows (MSVC, Release) | Build → asset check → CPU tests |

Run the full local demo:
```powershell
# Windows
powershell -ExecutionPolicy Bypass -File demo\run_demo.ps1
```
```bash
# Linux / macOS
bash demo/run_demo.sh
```

---

## Architecture Overview

```
main()
  └── Application (PIMPL)
        ├── SdlContext          SDL_Init / SDL_Quit RAII
        ├── SdlWindow           Window + GL context; loads GLAD
        ├── SdlInput            SDL events → abstract InputState
        ├── ShaderProgram       Linked GLSL program (move-only)
        ├── Texture2D           Procedural checkerboard (move-only)
        ├── Renderer2D          Dynamic batch renderer
        │     ├── VertexBuffer  GL_ARRAY_BUFFER (move-only)
        │     ├── IndexBuffer   GL_ELEMENT_ARRAY_BUFFER (move-only)
        │     └── VertexArray   VAO (move-only)
        ├── Simulation          Deterministic CPU-only physics
        ├── FixedTimestep       Clock accumulator; returns update count + alpha
        └── FrameProfiler       Per-phase timing; CSV export
```

Detailed component responsibilities and lifetime diagrams are in [docs/architecture.md](docs/architecture.md).

### Game-Loop Explanation

```cpp
while (running) {
    processEvents();                         // fill InputState

    accumulator += clamp(frameTime, maxDelta);
    int updates = 0;
    while (accumulator >= fixedDelta && updates < maxUpdates) {
        simulation.update(fixedDelta, input);
        accumulator -= fixedDelta;
        ++updates;
    }

    const double alpha = accumulator / fixedDelta;
    renderer.render(simulation.sprites(), alpha);  // interpolated positions
    window.swapBuffers();
}
```

**Spiral-of-death prevention:**
- Frame delta is clamped to 250 ms before entering the accumulator.
- At most 5 fixed steps are run per rendered frame.
- Excess accumulated time is silently discarded when both limits are hit.

### Fixed vs. Variable Timestep

| | Fixed | Variable |
|---|---|---|
| **Used for** | Simulation physics | Rendering |
| **dt per step** | Constant (1/60 s) | Real elapsed time |
| **Deterministic?** | Yes (same seed + inputs = same result) | No |
| **Affected by FPS?** | No | Yes |

The simulation runs at the fixed rate regardless of rendered FPS. The renderer
uses the `alpha` interpolation factor to smoothly blend between the previous
and current simulation positions.

### OpenGL Resource Ownership

All OpenGL handles are managed by move-only RAII wrappers. Copy construction
and copy assignment are deleted to prevent aliased deletion. Moved-from
objects hold `m_id = 0`; their destructors skip the `glDelete*` call.

The member declaration order in `Application::Impl` ensures all GL wrappers
are destroyed before `~SdlWindow()` invalidates the context.
See [docs/architecture.md](docs/architecture.md) for the full lifetime diagram.

### Shader Hot-Reload

Pressing **F5** triggers:
1. Read updated `.vert` and `.frag` source from disk.
2. Compile into **temporary** GL shader objects.
3. Link into a **temporary** program.
4. On **success**: move the new program into `m_shader`; delete the old one.
5. On **failure**: delete the temporaries; keep the existing working program active.

The renderer never renders without a valid shader, even after a failed reload.

---

## Repository Structure

```
RenderLoopLab/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── .gitignore / .editorconfig / .clang-format
├── Jenkinsfile
├── cmake/
│   ├── CompilerWarnings.cmake
│   └── Sanitizers.cmake
├── assets/shaders/
│   ├── sprite.vert / sprite.frag
│   └── examples/broken_sprite.frag.example
├── third_party/glad/         Pre-generated GL 3.3 Core loader
├── include/renderlooplab/
│   ├── application.hpp / application_config.hpp
│   ├── benchmark_report.hpp / cli.hpp
│   ├── fixed_timestep.hpp / frame_profiler.hpp
│   ├── input.hpp / logging.hpp / simulation.hpp / version.hpp
│   ├── graphics/   (buffer, renderer_2d, shader_program, texture_2d, vertex_array)
│   └── platform/   (sdl_context, sdl_input, sdl_window)
├── src/            Mirror of include/ with .cpp implementations
├── tests/          Catch2 unit tests (CPU-only)
├── docs/
│   ├── architecture.md
│   └── benchmarking.md
├── .github/workflows/ci.yml
└── .vscode/        Shared IDE configuration
```

---

## Build Prerequisites

| Tool | Required | Notes |
|---|---|---|
| CMake ≥ 3.21 | Yes | |
| C++17 compiler | Yes | MSVC ≥ 19.14, GCC ≥ 8, Clang ≥ 7 |
| Git | Yes | For FetchContent |
| OpenGL runtime | Yes | Typically provided by GPU driver |
| libgl-dev (Linux) | Yes | `libgl1-mesa-dev` |
| X11 dev (Linux) | Yes | `libx11-dev`, `libxrandr-dev`, etc. |
| xvfb (CI/Linux) | CI only | For headless OpenGL |

All other dependencies (SDL2, GLM, Catch2) are automatically downloaded and
built via CMake FetchContent.

---

## Windows Build

```bat
git clone https://github.com/OmriL997/RenderLoopLab.git
cd RenderLoopLab

cmake -B build -DRLL_BUILD_TESTS=ON
cmake --build build --config Release --parallel

build\bin\Release\renderlooplab.exe
```

### Run tests (Windows)
```bat
ctest --test-dir build -C Release --output-on-failure
```

---

## Linux Build

```bash
# Install prerequisites (Debian/Ubuntu)
sudo apt-get install -y build-essential cmake ninja-build \
    libgl1-mesa-dev libglu1-mesa-dev \
    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev \
    xvfb

git clone https://github.com/OmriL997/RenderLoopLab.git
cd RenderLoopLab

cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DRLL_BUILD_TESTS=ON
cmake --build build --parallel

./build/bin/renderlooplab
```

### Run tests (Linux)
```bash
ctest --test-dir build --output-on-failure
```

---

## Visual Studio Code

1. Install recommended extensions (prompted automatically from `.vscode/extensions.json`).
2. Open the workspace root in VS Code.
3. CMake Tools will detect the project and prompt to configure.
4. Use **Ctrl+Shift+B** (default build task) or the CMake Tools status bar.

Predefined tasks in `.vscode/tasks.json`:
- **CMake: Configure / Build** (Debug and Release)
- **Run: Unit Tests**
- **Run: Interactive Demo**
- **Run: Benchmark**
- **Run: Smoke Test**

---

## Usage

### Interactive mode
```bash
./renderlooplab
```

### Benchmark mode
```bash
./renderlooplab --benchmark \
    --sprites 1000 \
    --frames 3000 \
    --warmup 120 \
    --seed 12345 \
    --no-vsync \
    --hidden \
    --csv benchmark.csv \
    --report benchmark.json
```

### Smoke-test mode (CI)
```bash
xvfb-run -a ./renderlooplab --smoke-test --frames 120 --hidden --sprites 100
```

### Help
```bash
./renderlooplab --help
```

### Exit codes
| Code | Meaning |
|---|---|
| 0 | Success |
| 1 | Command-line / configuration error |
| 2 | SDL / OpenGL / shader initialisation failure |
| 3 | Benchmark report or CSV write failure |
| 4 | Smoke-test runtime failure |

---

## Testing

```bash
# Run all CPU tests
ctest --test-dir build --output-on-failure

# Run with verbose output
./build/bin/renderlooplab_tests --reporter console
```

Test coverage includes:
- Fixed timestep accumulator, clamping, alpha range, max-updates cap
- Simulation determinism, boundary behaviour, player movement, interpolation
- Input state transitions, pressed vs. held, diagonal normalisation
- Frame profiler statistics (mean, median, percentiles, stddev)
- CLI parsing, validation, and error cases
- JSON/CSV report writing and escaping
- OpenGL wrapper ownership traits (compile-time `static_assert`)

---

## CI and Jenkins

**GitHub Actions** (`.github/workflows/ci.yml`) runs on push and pull request:
- **Ubuntu**: configure → build → asset verification → CPU tests → smoke test → short benchmark
- **Ubuntu (sanitizers)**: CPU tests with AddressSanitizer + UBSan
- **Windows**: configure → build → asset verification → CPU tests

**Jenkinsfile** provides an equivalent declarative pipeline for internal
Jenkins servers, with `xvfb-run` used on Unix agents for runtime stages.

---

## Dependency Versions

| Library | Version | Source |
|---|---|---|
| SDL2 | release-2.30.9 | GitHub FetchContent |
| GLM | 1.0.1 | GitHub FetchContent |
| Catch2 | v3.7.1 | GitHub FetchContent |
| GLAD | pre-generated (GL 3.3 Core) | Bundled in `third_party/glad/` |

---

## Known Limitations

- Window-title FPS display updates approximately 4 times per second, not every frame.
- Controller hot-plug is supported on SDL2 platforms that report `SDL_CONTROLLERDEVICEADDED`.
- On some Linux setups, adaptive VSync (swap interval −1) may fall back to standard VSync.
- The bundled GLAD loader covers OpenGL 3.3 Core functions used by this project;
  functions not listed in `third_party/glad/include/glad/glad.h` are not loaded.
- The benchmark does not control for thermal throttling or background system load.
- No runtime verification that the GL version is actually 3.3 or higher beyond GLAD's load check.

---

## Potential Future Improvements

- Texture atlas support for varied sprite appearances
- GPU instanced rendering path for higher sprite counts
- Automatic shader file-watcher for hot reload on save
- Frame pacing using `SDL_GL_GetSwapInterval` feedback
- Per-category log levels and optional log file output
- CMakePresets.json for one-command configure
- Clang-tidy integration in CI
