#include "renderlooplab/graphics/shader_program.hpp"
#include "renderlooplab/logging.hpp"

#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

namespace rll::gfx {

// ---------------------------------------------------------------------------
// Destructor / move
// ---------------------------------------------------------------------------

ShaderProgram::~ShaderProgram()
{
    if (m_id != 0)
        glDeleteProgram(m_id);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : m_id(other.m_id)
{
    other.m_id = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    if (this != &other) {
        if (m_id != 0)
            glDeleteProgram(m_id);
        m_id       = other.m_id;
        other.m_id = 0;
    }
    return *this;
}

// ---------------------------------------------------------------------------
// Internal: compile one shader stage
// ---------------------------------------------------------------------------

GLuint ShaderProgram::compileShader(GLenum type, std::string_view source,
                                     std::string_view name, std::string& errorOut)
{
    const GLuint id = glCreateShader(type);
    const auto*  src = source.data();
    const auto   len = static_cast<GLint>(source.size());
    glShaderSource(id, 1, &src, &len);
    glCompileShader(id);

    GLint ok = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLint logLen = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
        std::string log(static_cast<std::size_t>(logLen), '\0');
        glGetShaderInfoLog(id, logLen, nullptr, log.data());
        errorOut  = "Shader compile error in '";
        errorOut += std::string(name);
        errorOut += "':\n";
        errorOut += log;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

// ---------------------------------------------------------------------------
// Factory: from source strings
// ---------------------------------------------------------------------------

bool ShaderProgram::createFromSource(
    std::string_view vertSrc,
    std::string_view fragSrc,
    std::string_view vertName,
    std::string_view fragName,
    ShaderProgram&   out,
    std::string&     errorOut)
{
    std::string err;

    const GLuint vert = compileShader(GL_VERTEX_SHADER,   vertSrc, vertName, err);
    if (vert == 0) { errorOut = err; return false; }

    const GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSrc, fragName, err);
    if (frag == 0) {
        glDeleteShader(vert);
        errorOut = err;
        return false;
    }

    const GLuint prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);

    // Intermediate shaders are no longer needed regardless of link result.
    glDetachShader(prog, vert);
    glDetachShader(prog, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);

    GLint ok = GL_FALSE;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint logLen = 0;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLen);
        std::string log(static_cast<std::size_t>(logLen), '\0');
        glGetProgramInfoLog(prog, logLen, nullptr, log.data());
        glDeleteProgram(prog);
        errorOut  = "Program link error ('";
        errorOut += std::string(vertName) + "' + '" + std::string(fragName) + "'):\n";
        errorOut += log;
        return false;
    }

    // Replace out only on success (keeps old program if called for hot-reload).
    ShaderProgram tmp;
    tmp.m_id = prog;
    out = std::move(tmp);
    return true;
}

// ---------------------------------------------------------------------------
// Factory: from file paths
// ---------------------------------------------------------------------------

bool ShaderProgram::createFromFiles(
    std::string_view vertPath,
    std::string_view fragPath,
    ShaderProgram&   out,
    std::string&     errorOut)
{
    auto readFile = [&](std::string_view path, std::string& content) -> bool {
        std::ifstream f(std::string(path), std::ios::binary);
        if (!f.is_open()) {
            errorOut = "Cannot open shader file: ";
            errorOut += std::string(path);
            return false;
        }
        std::ostringstream ss;
        ss << f.rdbuf();
        content = ss.str();
        return true;
    };

    std::string vertSrc, fragSrc;
    if (!readFile(vertPath, vertSrc)) return false;
    if (!readFile(fragPath, fragSrc)) return false;

    return createFromSource(vertSrc, fragSrc, vertPath, fragPath, out, errorOut);
}

// ---------------------------------------------------------------------------
// Bind
// ---------------------------------------------------------------------------

void ShaderProgram::use() const
{
    glUseProgram(m_id);
}

// ---------------------------------------------------------------------------
// Uniform setters
// ---------------------------------------------------------------------------

void ShaderProgram::setInt(std::string_view name, int v) const
{
    glUniform1i(glGetUniformLocation(m_id, name.data()), v);
}

void ShaderProgram::setFloat(std::string_view name, float v) const
{
    glUniform1f(glGetUniformLocation(m_id, name.data()), v);
}

void ShaderProgram::setVec2(std::string_view name, const glm::vec2& v) const
{
    glUniform2f(glGetUniformLocation(m_id, name.data()), v.x, v.y);
}

void ShaderProgram::setVec3(std::string_view name, const glm::vec3& v) const
{
    glUniform3f(glGetUniformLocation(m_id, name.data()), v.x, v.y, v.z);
}

void ShaderProgram::setVec4(std::string_view name, const glm::vec4& v) const
{
    glUniform4f(glGetUniformLocation(m_id, name.data()), v.x, v.y, v.z, v.w);
}

void ShaderProgram::setMat4(std::string_view name, const glm::mat4& m) const
{
    glUniformMatrix4fv(
        glGetUniformLocation(m_id, name.data()),
        1, GL_FALSE, glm::value_ptr(m));
}

} // namespace rll::gfx
