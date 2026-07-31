#pragma once

#include <glad/glad.h>
#include <cstdint>

namespace rll::gfx {

/**
 * Move-only RAII wrapper for a 2D OpenGL texture.
 *
 * Ownership invariant: same as VertexBuffer.
 * Must be destroyed before the OpenGL context (see docs/architecture.md).
 */
class Texture2D {
public:
    Texture2D() = default;
    ~Texture2D();

    Texture2D(const Texture2D&)            = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;

    // ── Factory methods ──────────────────────────────────────────────────
    /// Upload RGBA8 pixel data.
    static Texture2D fromRGBA(int width, int height, const uint8_t* data);

    /**
     * Generate a checkerboard texture procedurally.
     *
     * @param cellsPerSide  Number of cells per side of the grid.
     * @param cellPixels    Size in pixels of each cell.
     *
     * The texture will be (cellsPerSide * cellPixels) × (cellsPerSide * cellPixels).
     * Light cells are (220, 220, 220, 255); dark cells are (60, 60, 60, 255).
     */
    static Texture2D createCheckerboard(int cellsPerSide = 4, int cellPixels = 8);

    // ── Bind / unbind ────────────────────────────────────────────────────
    void bind  (int slot = 0) const;
    void unbind()             const;

    // ── Accessors ────────────────────────────────────────────────────────
    int    width () const { return m_width;  }
    int    height() const { return m_height; }
    GLuint handle() const { return m_id;     }
    bool   valid () const { return m_id != 0;}

private:
    GLuint m_id     = 0;
    int    m_width  = 0;
    int    m_height = 0;
};

} // namespace rll::gfx
