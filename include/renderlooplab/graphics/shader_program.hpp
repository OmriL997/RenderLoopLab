#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <string_view>

namespace rll::gfx {

/**
 * Move-only RAII wrapper for a linked OpenGL shader program.
 *
 * Construction:
 *   Use ShaderProgram::createFromFiles() or createFromSource().
 *   Both return true on success and fill `out`; on failure they leave `out`
 *   unchanged and fill errorOut with a human-readable diagnostics string
 *   containing file paths and the full compiler/linker log.
 *
 * Intermediate shader objects (GLuint from glCreateShader) are always
 * deleted before the factory functions return, even on failure.
 *
 * Moved-from objects have handle() == 0 and valid() == false.
 */
class ShaderProgram {
public:
    ShaderProgram() = default;
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&)            = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    // ── Factory methods ─────────────────────────────────────────────────
    static bool createFromFiles(
        std::string_view vertPath,
        std::string_view fragPath,
        ShaderProgram&   out,
        std::string&     errorOut);

    static bool createFromSource(
        std::string_view vertSrc,
        std::string_view fragSrc,
        std::string_view vertName,
        std::string_view fragName,
        ShaderProgram&   out,
        std::string&     errorOut);

    // ── Render-state helpers ────────────────────────────────────────────
    void use() const;

    // ── Uniform setters (program must be bound with use()) ──────────────
    void setInt   (std::string_view name, int v)             const;
    void setFloat (std::string_view name, float v)           const;
    void setVec2  (std::string_view name, const glm::vec2& v)const;
    void setVec3  (std::string_view name, const glm::vec3& v)const;
    void setVec4  (std::string_view name, const glm::vec4& v)const;
    void setMat4  (std::string_view name, const glm::mat4& m)const;

    GLuint handle() const { return m_id; }
    bool   valid () const { return m_id != 0; }

private:
    GLuint m_id = 0;

    static GLuint compileShader(GLenum           type,
                                std::string_view source,
                                std::string_view name,
                                std::string&     errorOut);
};

} // namespace rll::gfx
