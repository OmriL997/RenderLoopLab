#pragma once

#include <glad/glad.h>

namespace rll::gfx {

/**
 * Move-only RAII wrapper for an OpenGL Vertex Array Object (VAO).
 *
 * Ownership invariant: same as VertexBuffer / IndexBuffer.
 * Must be destroyed before the OpenGL context (see docs/architecture.md).
 */
class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    VertexArray(const VertexArray&)            = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    void bind  ();         // non-const: lazily creates the VAO on first call
    void unbind() const;

    GLuint handle() const { return m_id; }
    bool   valid () const { return m_id != 0; }

private:
    GLuint m_id = 0;
};

} // namespace rll::gfx
