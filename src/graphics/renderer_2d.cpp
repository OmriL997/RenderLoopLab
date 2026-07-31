#include "renderlooplab/graphics/renderer_2d.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <vector>

namespace rll::gfx {

// ---------------------------------------------------------------------------
// Initialisation
// ---------------------------------------------------------------------------

bool Renderer2D::init(const ShaderProgram& shader, const Texture2D& texture)
{
    m_shader  = &shader;
    m_texture = &texture;

    // ── Pre-compute index buffer ─────────────────────────────────────────
    // Each quad = 4 vertices, 2 triangles, 6 indices.
    // Pattern: 0,1,2, 2,3,0  (CCW winding)
    std::vector<GLuint> indices;
    indices.reserve(static_cast<std::size_t>(MAX_SPRITES * 6));
    for (int i = 0; i < MAX_SPRITES; ++i) {
        const GLuint base = static_cast<GLuint>(i) * 4u;
        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
        indices.push_back(base + 0);
    }

    // ── Allocate GPU resources ───────────────────────────────────────────
    // VAO must be bound first so it captures the VBO/IBO bindings.
    m_vao.bind();

    // VBO – allocate maximum size, fill with zeros initially.
    m_vbo.upload(nullptr,
                 static_cast<std::size_t>(MAX_SPRITES * 4) * sizeof(Vertex),
                 GL_DYNAMIC_DRAW);

    // IBO – static, never changes.
    m_ibo.bind();
    m_ibo.upload(indices.data(),
                 indices.size() * sizeof(GLuint),
                 GL_STATIC_DRAW);

    // ── Vertex attribute pointers ────────────────────────────────────────
    // location 0: vec2 position (offset 0, stride 32)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          static_cast<GLsizei>(sizeof(Vertex)),
                          reinterpret_cast<const void*>(offsetof(Vertex, position)));

    // location 1: vec2 texCoord (offset 8, stride 32)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          static_cast<GLsizei>(sizeof(Vertex)),
                          reinterpret_cast<const void*>(offsetof(Vertex, texCoord)));

    // location 2: vec4 color (offset 16, stride 32)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
                          static_cast<GLsizei>(sizeof(Vertex)),
                          reinterpret_cast<const void*>(offsetof(Vertex, color)));

    m_vao.unbind();

    // Reserve CPU-side vertex batch
    m_vertices.reserve(static_cast<std::size_t>(MAX_SPRITES * 4));

    return true;
}

// ---------------------------------------------------------------------------
// Projection
// ---------------------------------------------------------------------------

void Renderer2D::setProjection(int width, int height)
{
    // Orthographic projection: origin top-left, y-down.
    m_projection = glm::ortho(
        0.0f, static_cast<float>(width),
        static_cast<float>(height), 0.0f,
        -1.0f, 1.0f);
}

// ---------------------------------------------------------------------------
// Per-frame API
// ---------------------------------------------------------------------------

void Renderer2D::beginFrame()
{
    m_vertices.clear();
    m_spriteCount = 0;
    m_frameStats  = {};
}

void Renderer2D::drawSprite(const SpriteDrawCmd& cmd)
{
    if (m_spriteCount >= MAX_SPRITES) return;

    const float hw = cmd.size.x * 0.5f;
    const float hh = cmd.size.y * 0.5f;

    // Local-space corners (CCW): BL, BR, TR, TL
    const glm::vec2 corners[4] = {
        {-hw, -hh},
        { hw, -hh},
        { hw,  hh},
        {-hw,  hh}
    };

    const float cosR = std::cos(cmd.rotation);
    const float sinR = std::sin(cmd.rotation);

    const glm::vec2 uvs[4] = {
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f}
    };

    for (int v = 0; v < 4; ++v) {
        const float lx = corners[v].x;
        const float ly = corners[v].y;
        const float wx = cosR * lx - sinR * ly + cmd.position.x;
        const float wy = sinR * lx + cosR * ly + cmd.position.y;

        Vertex vtx;
        vtx.position = {wx, wy};
        vtx.texCoord = uvs[v];
        vtx.color    = cmd.color;
        m_vertices.push_back(vtx);
    }

    ++m_spriteCount;
}

RenderStats Renderer2D::endFrame()
{
    if (m_spriteCount > 0) flush();
    return m_frameStats;
}

void Renderer2D::flush()
{
    if (m_vertices.empty()) return;

    // Upload vertices to GPU
    m_vbo.bind();
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    static_cast<GLsizeiptr>(m_vertices.size() * sizeof(Vertex)),
                    m_vertices.data());

    // Bind state
    m_shader->use();
    m_shader->setMat4("u_Projection", m_projection);
    m_shader->setInt ("u_Texture", 0);
    m_texture->bind(0);
    m_vao.bind();

    // Draw all quads in one call
    glDrawElements(GL_TRIANGLES,
                   static_cast<GLsizei>(m_spriteCount * 6),
                   GL_UNSIGNED_INT,
                   nullptr);

    m_vao.unbind();

    m_frameStats.drawCalls++;
    m_frameStats.triangles += m_spriteCount * 2;
    m_frameStats.sprites   += m_spriteCount;
}

bool Renderer2D::valid() const
{
    return m_shader  != nullptr && m_shader->valid()  &&
           m_texture != nullptr && m_texture->valid() &&
           m_vao.valid();
}

} // namespace rll::gfx
