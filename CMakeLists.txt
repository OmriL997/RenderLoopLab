cmake_minimum_required(VERSION 3.21)

project(RenderLoopLab
    VERSION 1.0.0
    DESCRIPTION "Real-time 2D rendering and game-loop laboratory"
    LANGUAGES C CXX
)

# ── C++17 ──────────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Output directories ──────────────────────────────────────────────────────
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# ── Options ─────────────────────────────────────────────────────────────────
option(RLL_BUILD_TESTS             "Build the Catch2 unit-test suite"          ON)
option(RLL_WARNINGS_AS_ERRORS      "Treat project warnings as errors"          OFF)
option(RLL_ENABLE_SANITIZERS       "Enable ASan + UBSan (GCC/Clang only)"     OFF)
option(RLL_BUILD_RUNTIME_SMOKE_TEST "Build smoke-test CTest entry"            OFF)

# ── CMake helpers ────────────────────────────────────────────────────────────
include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)
include(FetchContent)

# ── Dependencies ─────────────────────────────────────────────────────────────

# SDL2 – static build
FetchContent_Declare(SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-2.30.9
    GIT_SHALLOW    TRUE
)
# Force static library; suppress SDL's own tests and examples
set(SDL_SHARED      OFF CACHE BOOL "" FORCE)
set(SDL_STATIC      ON  CACHE BOOL "" FORCE)
set(SDL_TEST        OFF CACHE BOOL "" FORCE)
set(SDL2_DISABLE_SDL2MAIN OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2)

# GLM – header-only
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
    GIT_SHALLOW    TRUE
)
set(GLM_ENABLE_CXX_17 ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glm)

# Catch2 (only when tests are enabled)
if(RLL_BUILD_TESTS)
    FetchContent_Declare(Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.7.1
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(Catch2)
    list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
    include(CTest)
    include(Catch)
endif()

# ── GLAD (bundled pre-generated, no Python dependency) ──────────────────────
add_library(glad STATIC
    third_party/glad/src/glad.c
)
target_include_directories(glad
    PUBLIC  third_party/glad/include
)
# Suppress warnings from the bundled C code
if(MSVC)
    target_compile_options(glad PRIVATE /W0)
else()
    target_compile_options(glad PRIVATE -w)
endif()

# ── renderlooplab_core  (CPU-only, no OpenGL, no SDL) ───────────────────────
add_library(renderlooplab_core STATIC
    src/logging.cpp
    src/cli.cpp
    src/fixed_timestep.cpp
    src/input.cpp
    src/simulation.cpp
    src/frame_profiler.cpp
    src/benchmark_report.cpp
)
target_include_directories(renderlooplab_core
    PUBLIC  include
)
target_link_libraries(renderlooplab_core
    PUBLIC  glm::glm
)
rll_set_warnings(renderlooplab_core)
rll_enable_sanitizers(renderlooplab_core)

# ── renderlooplab_graphics (OpenGL + SDL) ────────────────────────────────────
add_library(renderlooplab_graphics STATIC
    src/graphics/buffer.cpp
    src/graphics/vertex_array.cpp
    src/graphics/shader_program.cpp
    src/graphics/texture_2d.cpp
    src/graphics/renderer_2d.cpp
    src/platform/sdl_context.cpp
    src/platform/sdl_window.cpp
    src/platform/sdl_input.cpp
)
target_include_directories(renderlooplab_graphics
    PUBLIC  include
)
target_link_libraries(renderlooplab_graphics
    PUBLIC  renderlooplab_core
    PUBLIC  glad
    PUBLIC  SDL2::SDL2-static
)
if(WIN32)
    target_link_libraries(renderlooplab_graphics PRIVATE opengl32)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(renderlooplab_graphics PRIVATE OpenGL::GL)
endif()
rll_set_warnings(renderlooplab_graphics)
rll_enable_sanitizers(renderlooplab_graphics)

# ── Executable ───────────────────────────────────────────────────────────────
add_executable(renderlooplab
    src/application.cpp
    src/main.cpp
)
target_link_libraries(renderlooplab
    PRIVATE renderlooplab_graphics
    PRIVATE renderlooplab_core
)
if(WIN32)
    # SDL2main provides the WinMain → main trampoline.
    target_link_libraries(renderlooplab PRIVATE SDL2::SDL2main)
endif()
rll_set_warnings(renderlooplab)
rll_enable_sanitizers(renderlooplab)

# On Windows link against SDL2 system libraries that SDL static needs
if(WIN32)
    target_link_libraries(renderlooplab PRIVATE
        winmm imm32 version setupapi
    )
endif()

# ── Asset copying ─────────────────────────────────────────────────────────────
# Copy the assets/ directory next to the built executable so that the binary
# can locate shaders without any knowledge of the source root.
add_custom_command(TARGET renderlooplab POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:renderlooplab>/assets"
    COMMENT "Copying assets to output directory"
)

# ── Tests ─────────────────────────────────────────────────────────────────────
if(RLL_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
