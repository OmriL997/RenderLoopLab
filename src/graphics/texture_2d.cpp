#include "renderlooplab/graphics/texture_2d.hpp"

#include <vector>

namespace rll::gfx {

// ---------------------------------------------------------------------------
// Destructor / move
// ---------------------------------------------------------------------------

Texture2D::~Texture2D()
{
    if (m_id != 0)
        glDeleteTextures(1, &m_id);
}

Texture2D::Texture2D(Texture2D&& other) noexcept
    : m_id(other.m_id), m_width(other.m_width), m_height(other.m_height)
{
    other.m_id     = 0;
    other.m_width  = 0;
    other.m_height = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
    if (this != &other) {
        if (m_id != 0)
            glDeleteTextures(1, &m_id);
        m_id       = other.m_id;
        m_width    = other.m_width;
        m_height   = other.m_height;
        other.m_id     = 0;
        other.m_width  = 0;
        other.m_height = 0;
    }
    return *this;
}

// ---------------------------------------------------------------------------
// Factory: from raw RGBA8 data
// ---------------------------------------------------------------------------

Texture2D Texture2D::fromRGBA(int width, int height, const uint8_t* data)
{
    Texture2D tex;
    glGenTextures(1, &tex.m_id);
    tex.m_width  = width;
    tex.m_height = height;

    glBindTexture(GL_TEXTURE_2D, tex.m_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,   GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,   GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    return tex;
}

// ---------------------------------------------------------------------------
// Factory: procedural checkerboard
// ---------------------------------------------------------------------------

Texture2D Texture2D::createCheckerboard(int cellsPerSide, int cellPixels)
{
    const int side   = cellsPerSide * cellPixels;
    const int pixels = side * side;

    // RGBA8
    std::vector<uint8_t> data(static_cast<std::size_t>(pixels * 4));

    constexpr uint8_t LIGHT[4] = {220, 220, 220, 255};
    constexpr uint8_t DARK [4] = { 60,  60,  60, 255};

    for (int y = 0; y < side; ++y) {
        for (int x = 0; x < side; ++x) {
            const int cellX = x / cellPixels;
            const int cellY = y / cellPixels;
            const bool light = (cellX + cellY) % 2 == 0;
            const uint8_t* col = light ? LIGHT : DARK;
            const int idx = (y * side + x) * 4;
            data[static_cast<std::size_t>(idx + 0)] = col[0];
            data[static_cast<std::size_t>(idx + 1)] = col[1];
            data[static_cast<std::size_t>(idx + 2)] = col[2];
            data[static_cast<std::size_t>(idx + 3)] = col[3];
        }
    }

    return fromRGBA(side, side, data.data());
}

// ---------------------------------------------------------------------------
// Bind / unbind
// ---------------------------------------------------------------------------

void Texture2D::bind(int slot) const
{
    glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(slot));
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture2D::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace rll::gfx
