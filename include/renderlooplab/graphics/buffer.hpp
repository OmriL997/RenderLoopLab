#pragma once

#include <glad/glad.h>
#include <cstddef>

namespace rll::gfx {

/**
 * Move-only RAII wrapper for an OpenGL vertex buffer (GL_ARRAY_BUFFER).
 *
 * Ownership invariant:
 *   m_id == 0  → no OpenGL resource owned (default / moved-from state)
 *   m_id != 0  → owns exactly one glGenBuffers handle
 *
 * Destructor calls glDeleteBuffers(1, &m_id) when m_id != 0.
 * Copy construction and copy assignment are deleted; no shared ownership.
 *
 * IMPORTANT: The owning object must be destroyed before the OpenGL context
 * is destroyed (see docs/architecture.md – "Resource Lifetime").
 */
class VertexBuffer {
public:
    VertexBuffer();
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&)            = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void bind   () const;
    void unbind () const;

    /// Allocate or replace buffer storage (full upload).
    void upload(const void* data, std::size_t byteSize,
                GLenum usage = GL_DYNAMIC_DRAW);

    /// Partial update — buffer must already be allocated with upload().
    void uploadSubData(const void* data, std::size_t offsetBytes,
                       std::size_t byteSize);

    GLuint handle() const { return m_id; }
    bool   valid () const { return m_id != 0; }

private:
    GLuint m_id = 0;
};

// ---------------------------------------------------------------------------

/**
 * Move-only RAII wrapper for an OpenGL index buffer (GL_ELEMENT_ARRAY_BUFFER).
 *
 * Same ownership contract as VertexBuffer.
 */
class IndexBuffer {
public:
    IndexBuffer();
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&)            = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    IndexBuffer(IndexBuffer&& other) noexcept;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;

    void bind  () const;
    void unbind() const;

    void upload(const void* data, std::size_t byteSize,
                GLenum usage = GL_STATIC_DRAW);

    GLuint handle() const { return m_id; }
    bool   valid () const { return m_id != 0; }

private:
    GLuint m_id = 0;
};

} // namespace rll::gfx
