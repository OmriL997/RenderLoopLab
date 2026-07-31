/**
 * resource_traits_tests.cpp
 *
 * Verifies at compile time that OpenGL RAII wrappers satisfy the required
 * ownership policy:
 *   - NOT copy constructible
 *   - NOT copy assignable
 *   - IS move constructible
 *   - IS move assignable
 *
 * No OpenGL context is required because we only use static_assert.
 */

#include <catch2/catch_test_macros.hpp>

#include "renderlooplab/graphics/buffer.hpp"
#include "renderlooplab/graphics/vertex_array.hpp"
#include "renderlooplab/graphics/shader_program.hpp"
#include "renderlooplab/graphics/texture_2d.hpp"

using namespace rll::gfx;

// ── VertexBuffer ──────────────────────────────────────────────────────────
static_assert(!std::is_copy_constructible_v<VertexBuffer>,
              "VertexBuffer must not be copy constructible");
static_assert(!std::is_copy_assignable_v<VertexBuffer>,
              "VertexBuffer must not be copy assignable");
static_assert(std::is_move_constructible_v<VertexBuffer>,
              "VertexBuffer must be move constructible");
static_assert(std::is_move_assignable_v<VertexBuffer>,
              "VertexBuffer must be move assignable");

// ── IndexBuffer ───────────────────────────────────────────────────────────
static_assert(!std::is_copy_constructible_v<IndexBuffer>,
              "IndexBuffer must not be copy constructible");
static_assert(!std::is_copy_assignable_v<IndexBuffer>,
              "IndexBuffer must not be copy assignable");
static_assert(std::is_move_constructible_v<IndexBuffer>,
              "IndexBuffer must be move constructible");
static_assert(std::is_move_assignable_v<IndexBuffer>,
              "IndexBuffer must be move assignable");

// ── VertexArray ───────────────────────────────────────────────────────────
static_assert(!std::is_copy_constructible_v<VertexArray>,
              "VertexArray must not be copy constructible");
static_assert(!std::is_copy_assignable_v<VertexArray>,
              "VertexArray must not be copy assignable");
static_assert(std::is_move_constructible_v<VertexArray>,
              "VertexArray must be move constructible");
static_assert(std::is_move_assignable_v<VertexArray>,
              "VertexArray must be move assignable");

// ── ShaderProgram ─────────────────────────────────────────────────────────
static_assert(!std::is_copy_constructible_v<ShaderProgram>,
              "ShaderProgram must not be copy constructible");
static_assert(!std::is_copy_assignable_v<ShaderProgram>,
              "ShaderProgram must not be copy assignable");
static_assert(std::is_move_constructible_v<ShaderProgram>,
              "ShaderProgram must be move constructible");
static_assert(std::is_move_assignable_v<ShaderProgram>,
              "ShaderProgram must be move assignable");

// ── Texture2D ─────────────────────────────────────────────────────────────
static_assert(!std::is_copy_constructible_v<Texture2D>,
              "Texture2D must not be copy constructible");
static_assert(!std::is_copy_assignable_v<Texture2D>,
              "Texture2D must not be copy assignable");
static_assert(std::is_move_constructible_v<Texture2D>,
              "Texture2D must be move constructible");
static_assert(std::is_move_assignable_v<Texture2D>,
              "Texture2D must be move assignable");

// ── Runtime test (ensures the static_asserts above are actually checked) ──
TEST_CASE("OpenGL wrappers satisfy move-only ownership traits", "[resource_traits]")
{
    // All verification is done at compile time via static_assert above.
    // This test case simply exists to appear in the CTest output.
    SUCCEED("All static_assert checks passed at compile time.");
}
