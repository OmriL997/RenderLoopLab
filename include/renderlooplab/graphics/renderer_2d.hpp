#pragma once

#include "renderlooplab/graphics/buffer.hpp"
#include "renderlooplab/graphics/vertex_array.hpp"
#include "renderlooplab/graphics/shader_program.hpp"
#include "renderlooplab/graphics/texture_2d.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <cstddef>

namespace rll::gfx {

struct RenderStats {
    int drawCalls = 0;
    int triangles = 0;
    int sprites   = 0;
};

/// Command submitted once per sprite per frame.
struct SpriteDrawCmd {
    glm::vec2 position;   ///< Centre position in world/screen space
    glm::vec2 size;       ///< Width × Height
    float     rotation;   ///< Radians, counter-clockwise
    glm::vec4 color;      ///< RGBA tint (1,1,1,1 = no tint)
};

/**
 * Dynamic-batch 2D sprite renderer.
 *
 * All visible sprites are collected into a single CPU-side vertex buffer,
 * uploaded to the GPU once per frame, and drawn with one glDrawElements call.
 *
 * Vertex layout (interleaved, 32 bytes per vertex):
 *   location 0: vec2 position
 *   location 1: vec2 texCoord
 *   location 2: vec4 color
 *
 * Each sprite is a quad: 4 vertices / 6 indices (2 triangles).
 *
 * Maximum sprites per frame: MAX_SPRITES (10 000 by default).
 *
 * Shaders and texture are owned externally; Renderer2D holds non-owning
 * const pointers to them.  The caller is responsible for shader/texture
 * lifetime exceeding the renderer's lifetime.
 */
class Renderer2D {
public:
    static constexpr int MAX_SPRITES = 10000;

    Renderer2D() = default;

    /// Allocate GPU resources. Call once after the OpenGL context is ready.
    /// @param shader   A valid, linked ShaderProgram.
    /// @param texture  A valid Texture2D.
    /// @return true on success.
    bool init(const ShaderProgram& shader, const Texture2D& texture);

    /// Set/update the orthographic projection to cover [0,w]×[0,h].
    /// Call when the drawable size changes.
    void setProjection(int width, int height);

    // ── Per-frame API ───────────────────────────────────────────────────
    void       beginFrame();
    void       drawSprite(const SpriteDrawCmd& cmd);
    RenderStats endFrame();          ///< Flushes and issues draw call(s)

    bool valid() const;

private:
    struct Vertex {
        glm::vec2 position;
        glm::vec2 texCoord;
        glm::vec4 color;
    };
    static_assert(sizeof(Vertex) == 32, "Vertex layout must be 32 bytes");

    // GPU resources
    VertexBuffer m_vbo;
    IndexBuffer  m_ibo;
    VertexArray  m_vao;

    // External refs (non-owning)
    const ShaderProgram* m_shader  = nullptr;
    const Texture2D*     m_texture = nullptr;

    glm::mat4         m_projection{1.0f};
    std::vector<Vertex> m_vertices;       ///< CPU-side batch
    int               m_spriteCount = 0;
    RenderStats       m_frameStats{};

    void flush();
};

} // namespace rll::gfx
