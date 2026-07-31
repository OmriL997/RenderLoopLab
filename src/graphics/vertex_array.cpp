#include "renderlooplab/graphics/vertex_array.hpp"

namespace rll::gfx {

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
    if (m_id != 0)
        glDeleteVertexArrays(1, &m_id);
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_id(other.m_id)
{
    other.m_id = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (this != &other) {
        if (m_id != 0)
            glDeleteVertexArrays(1, &m_id);
        m_id       = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

void VertexArray::bind() const
{
    glBindVertexArray(m_id);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

} // namespace rll::gfx
