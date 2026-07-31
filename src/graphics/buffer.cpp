#include "renderlooplab/graphics/buffer.hpp"

namespace rll::gfx {

// ---------------------------------------------------------------------------
// VertexBuffer
// ---------------------------------------------------------------------------

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_id);
}

VertexBuffer::~VertexBuffer()
{
    if (m_id != 0)
        glDeleteBuffers(1, &m_id);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_id(other.m_id)
{
    other.m_id = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    if (this != &other) {
        if (m_id != 0)
            glDeleteBuffers(1, &m_id);
        m_id       = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::upload(const void* data, std::size_t byteSize, GLenum usage)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(byteSize),
                 data, usage);
}

void VertexBuffer::uploadSubData(const void* data, std::size_t offsetBytes,
                                  std::size_t byteSize)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferSubData(GL_ARRAY_BUFFER,
                    static_cast<GLintptr>(offsetBytes),
                    static_cast<GLsizeiptr>(byteSize),
                    data);
}

// ---------------------------------------------------------------------------
// IndexBuffer
// ---------------------------------------------------------------------------

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &m_id);
}

IndexBuffer::~IndexBuffer()
{
    if (m_id != 0)
        glDeleteBuffers(1, &m_id);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : m_id(other.m_id)
{
    other.m_id = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
    if (this != &other) {
        if (m_id != 0)
            glDeleteBuffers(1, &m_id);
        m_id       = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::upload(const void* data, std::size_t byteSize, GLenum usage)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(byteSize),
                 data, usage);
}

} // namespace rll::gfx
