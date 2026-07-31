/*
 * Pre-generated GLAD loader implementation for OpenGL 3.3 Core Profile
 * Bundled with RenderLoopLab.
 */

#include "glad/glad.h"

#include <stddef.h>
#include <string.h>

/* ========== Function pointer definitions ========== */

PFNGLVIEWPORTPROC                  glad_glViewport                  = NULL;
PFNGLCLEARPROC                     glad_glClear                     = NULL;
PFNGLCLEARCOLORPROC                glad_glClearColor                = NULL;
PFNGLENABLEPROC                    glad_glEnable                    = NULL;
PFNGLDISABLEPROC                   glad_glDisable                   = NULL;
PFNGLBLENDFUNCPROC                 glad_glBlendFunc                 = NULL;
PFNGLBLENDEQUATIONPROC             glad_glBlendEquation             = NULL;
PFNGLBLENDFUNCSEPARATEPROC         glad_glBlendFuncSeparate         = NULL;
PFNGLSCISSORPROC                   glad_glScissor                   = NULL;
PFNGLPIXELSTOREIPROC               glad_glPixelStorei               = NULL;
PFNGLDRAWELEMENTSPROC              glad_glDrawElements              = NULL;
PFNGLDRAWARRAYSPROC                glad_glDrawArrays                = NULL;
PFNGLGETSTRINGPROC                 glad_glGetString                 = NULL;
PFNGLGETSTRINGIPROC                glad_glGetStringi                = NULL;
PFNGLGETINTEGERVPROC               glad_glGetIntegerv               = NULL;
PFNGLGETERRORPROC                  glad_glGetError                  = NULL;

PFNGLGENTEXTURESPROC               glad_glGenTextures               = NULL;
PFNGLDELETETEXTURESPROC            glad_glDeleteTextures            = NULL;
PFNGLBINDTEXTUREPROC               glad_glBindTexture               = NULL;
PFNGLTEXIMAGE2DPROC                glad_glTexImage2D                = NULL;
PFNGLTEXSUBIMAGE2DPROC             glad_glTexSubImage2D             = NULL;
PFNGLTEXPARAMETERIPROC             glad_glTexParameteri             = NULL;
PFNGLTEXPARAMETERFPROC             glad_glTexParameterf             = NULL;
PFNGLGENERATEMIPMAPPROC            glad_glGenerateMipmap            = NULL;
PFNGLACTIVETEXTUREPROC             glad_glActiveTexture             = NULL;

PFNGLGENBUFFERSPROC                glad_glGenBuffers                = NULL;
PFNGLDELETEBUFFERSPROC             glad_glDeleteBuffers             = NULL;
PFNGLBINDBUFFERPROC                glad_glBindBuffer                = NULL;
PFNGLBUFFERDATAPROC                glad_glBufferData                = NULL;
PFNGLBUFFERSUBDATAPROC             glad_glBufferSubData             = NULL;
PFNGLMAPBUFFERPROC                 glad_glMapBuffer                 = NULL;
PFNGLUNMAPBUFFERPROC               glad_glUnmapBuffer               = NULL;

PFNGLGENVERTEXARRAYSPROC           glad_glGenVertexArrays           = NULL;
PFNGLDELETEVERTEXARRAYSPROC        glad_glDeleteVertexArrays        = NULL;
PFNGLBINDVERTEXARRAYPROC           glad_glBindVertexArray           = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC   glad_glEnableVertexAttribArray   = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC  glad_glDisableVertexAttribArray  = NULL;
PFNGLVERTEXATTRIBPOINTERPROC       glad_glVertexAttribPointer       = NULL;
PFNGLVERTEXATTRIBIPOINTERPROC      glad_glVertexAttribIPointer      = NULL;
PFNGLVERTEXATTRIBDIVISORPROC       glad_glVertexAttribDivisor       = NULL;
PFNGLDRAWELEMENTSINSTANCEDPROC     glad_glDrawElementsInstanced     = NULL;
PFNGLDRAWARRAYSINSTANCEDPROC       glad_glDrawArraysInstanced       = NULL;

PFNGLCREATESHADERPROC              glad_glCreateShader              = NULL;
PFNGLDELETESHADERPROC              glad_glDeleteShader              = NULL;
PFNGLSHADERSOURCEPROC              glad_glShaderSource              = NULL;
PFNGLCOMPILESHADERPROC             glad_glCompileShader             = NULL;
PFNGLGETSHADERIVPROC               glad_glGetShaderiv               = NULL;
PFNGLGETSHADERINFOLOGPROC          glad_glGetShaderInfoLog          = NULL;

PFNGLCREATEPROGRAMPROC             glad_glCreateProgram             = NULL;
PFNGLDELETEPROGRAMPROC             glad_glDeleteProgram             = NULL;
PFNGLATTACHSHADERPROC              glad_glAttachShader              = NULL;
PFNGLDETACHSHADERPROC              glad_glDetachShader              = NULL;
PFNGLLINKPROGRAMPROC               glad_glLinkProgram               = NULL;
PFNGLGETPROGRAMIVPROC              glad_glGetProgramiv              = NULL;
PFNGLGETPROGRAMINFOLOGPROC         glad_glGetProgramInfoLog         = NULL;
PFNGLUSEPROGRAMPROC                glad_glUseProgram                = NULL;

PFNGLGETUNIFORMLOCATIONPROC        glad_glGetUniformLocation        = NULL;
PFNGLUNIFORM1IPROC                 glad_glUniform1i                 = NULL;
PFNGLUNIFORM2IPROC                 glad_glUniform2i                 = NULL;
PFNGLUNIFORM3IPROC                 glad_glUniform3i                 = NULL;
PFNGLUNIFORM4IPROC                 glad_glUniform4i                 = NULL;
PFNGLUNIFORM1FPROC                 glad_glUniform1f                 = NULL;
PFNGLUNIFORM2FPROC                 glad_glUniform2f                 = NULL;
PFNGLUNIFORM3FPROC                 glad_glUniform3f                 = NULL;
PFNGLUNIFORM4FPROC                 glad_glUniform4f                 = NULL;
PFNGLUNIFORM1IVPROC                glad_glUniform1iv                = NULL;
PFNGLUNIFORM2IVPROC                glad_glUniform2iv                = NULL;
PFNGLUNIFORM3IVPROC                glad_glUniform3iv                = NULL;
PFNGLUNIFORM4IVPROC                glad_glUniform4iv                = NULL;
PFNGLUNIFORM1FVPROC                glad_glUniform1fv                = NULL;
PFNGLUNIFORM2FVPROC                glad_glUniform2fv                = NULL;
PFNGLUNIFORM3FVPROC                glad_glUniform3fv                = NULL;
PFNGLUNIFORM4FVPROC                glad_glUniform4fv                = NULL;
PFNGLUNIFORMMATRIX2FVPROC          glad_glUniformMatrix2fv          = NULL;
PFNGLUNIFORMMATRIX3FVPROC          glad_glUniformMatrix3fv          = NULL;
PFNGLUNIFORMMATRIX4FVPROC          glad_glUniformMatrix4fv          = NULL;

/* ========== Loader ========== */

static GLADloadproc glad_loader_fn = NULL;

static void* glad_get_proc(const char* name) {
    if (glad_loader_fn == NULL) return NULL;
    return glad_loader_fn(name);
}

#define LOAD(type, name) glad_##name = (type)glad_get_proc(#name)

int gladLoadGLLoader(GLADloadproc load) {
    glad_loader_fn = load;

    LOAD(PFNGLVIEWPORTPROC,                 glViewport);
    LOAD(PFNGLCLEARPROC,                    glClear);
    LOAD(PFNGLCLEARCOLORPROC,               glClearColor);
    LOAD(PFNGLENABLEPROC,                   glEnable);
    LOAD(PFNGLDISABLEPROC,                  glDisable);
    LOAD(PFNGLBLENDFUNCPROC,                glBlendFunc);
    LOAD(PFNGLBLENDEQUATIONPROC,            glBlendEquation);
    LOAD(PFNGLBLENDFUNCSEPARATEPROC,        glBlendFuncSeparate);
    LOAD(PFNGLSCISSORPROC,                  glScissor);
    LOAD(PFNGLPIXELSTOREIPROC,              glPixelStorei);
    LOAD(PFNGLDRAWELEMENTSPROC,             glDrawElements);
    LOAD(PFNGLDRAWARRAYSPROC,               glDrawArrays);
    LOAD(PFNGLGETSTRINGPROC,                glGetString);
    LOAD(PFNGLGETSTRINGIPROC,               glGetStringi);
    LOAD(PFNGLGETINTEGERVPROC,              glGetIntegerv);
    LOAD(PFNGLGETERRORPROC,                 glGetError);

    LOAD(PFNGLGENTEXTURESPROC,              glGenTextures);
    LOAD(PFNGLDELETETEXTURESPROC,           glDeleteTextures);
    LOAD(PFNGLBINDTEXTUREPROC,              glBindTexture);
    LOAD(PFNGLTEXIMAGE2DPROC,               glTexImage2D);
    LOAD(PFNGLTEXSUBIMAGE2DPROC,            glTexSubImage2D);
    LOAD(PFNGLTEXPARAMETERIPROC,            glTexParameteri);
    LOAD(PFNGLTEXPARAMETERFPROC,            glTexParameterf);
    LOAD(PFNGLGENERATEMIPMAPPROC,           glGenerateMipmap);
    LOAD(PFNGLACTIVETEXTUREPROC,            glActiveTexture);

    LOAD(PFNGLGENBUFFERSPROC,               glGenBuffers);
    LOAD(PFNGLDELETEBUFFERSPROC,            glDeleteBuffers);
    LOAD(PFNGLBINDBUFFERPROC,               glBindBuffer);
    LOAD(PFNGLBUFFERDATAPROC,               glBufferData);
    LOAD(PFNGLBUFFERSUBDATAPROC,            glBufferSubData);
    LOAD(PFNGLMAPBUFFERPROC,                glMapBuffer);
    LOAD(PFNGLUNMAPBUFFERPROC,              glUnmapBuffer);

    LOAD(PFNGLGENVERTEXARRAYSPROC,          glGenVertexArrays);
    LOAD(PFNGLDELETEVERTEXARRAYSPROC,       glDeleteVertexArrays);
    LOAD(PFNGLBINDVERTEXARRAYPROC,          glBindVertexArray);
    LOAD(PFNGLENABLEVERTEXATTRIBARRAYPROC,  glEnableVertexAttribArray);
    LOAD(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
    LOAD(PFNGLVERTEXATTRIBPOINTERPROC,      glVertexAttribPointer);
    LOAD(PFNGLVERTEXATTRIBIPOINTERPROC,     glVertexAttribIPointer);
    LOAD(PFNGLVERTEXATTRIBDIVISORPROC,      glVertexAttribDivisor);
    LOAD(PFNGLDRAWELEMENTSINSTANCEDPROC,    glDrawElementsInstanced);
    LOAD(PFNGLDRAWARRAYSINSTANCEDPROC,      glDrawArraysInstanced);

    LOAD(PFNGLCREATESHADERPROC,             glCreateShader);
    LOAD(PFNGLDELETESHADERPROC,             glDeleteShader);
    LOAD(PFNGLSHADERSOURCEPROC,             glShaderSource);
    LOAD(PFNGLCOMPILESHADERPROC,            glCompileShader);
    LOAD(PFNGLGETSHADERIVPROC,              glGetShaderiv);
    LOAD(PFNGLGETSHADERINFOLOGPROC,         glGetShaderInfoLog);

    LOAD(PFNGLCREATEPROGRAMPROC,            glCreateProgram);
    LOAD(PFNGLDELETEPROGRAMPROC,            glDeleteProgram);
    LOAD(PFNGLATTACHSHADERPROC,             glAttachShader);
    LOAD(PFNGLDETACHSHADERPROC,             glDetachShader);
    LOAD(PFNGLLINKPROGRAMPROC,              glLinkProgram);
    LOAD(PFNGLGETPROGRAMIVPROC,             glGetProgramiv);
    LOAD(PFNGLGETPROGRAMINFOLOGPROC,        glGetProgramInfoLog);
    LOAD(PFNGLUSEPROGRAMPROC,               glUseProgram);

    LOAD(PFNGLGETUNIFORMLOCATIONPROC,       glGetUniformLocation);
    LOAD(PFNGLUNIFORM1IPROC,                glUniform1i);
    LOAD(PFNGLUNIFORM2IPROC,                glUniform2i);
    LOAD(PFNGLUNIFORM3IPROC,                glUniform3i);
    LOAD(PFNGLUNIFORM4IPROC,                glUniform4i);
    LOAD(PFNGLUNIFORM1FPROC,                glUniform1f);
    LOAD(PFNGLUNIFORM2FPROC,                glUniform2f);
    LOAD(PFNGLUNIFORM3FPROC,                glUniform3f);
    LOAD(PFNGLUNIFORM4FPROC,                glUniform4f);
    LOAD(PFNGLUNIFORM1IVPROC,               glUniform1iv);
    LOAD(PFNGLUNIFORM2IVPROC,               glUniform2iv);
    LOAD(PFNGLUNIFORM3IVPROC,               glUniform3iv);
    LOAD(PFNGLUNIFORM4IVPROC,               glUniform4iv);
    LOAD(PFNGLUNIFORM1FVPROC,               glUniform1fv);
    LOAD(PFNGLUNIFORM2FVPROC,               glUniform2fv);
    LOAD(PFNGLUNIFORM3FVPROC,               glUniform3fv);
    LOAD(PFNGLUNIFORM4FVPROC,               glUniform4fv);
    LOAD(PFNGLUNIFORMMATRIX2FVPROC,         glUniformMatrix2fv);
    LOAD(PFNGLUNIFORMMATRIX3FVPROC,         glUniformMatrix3fv);
    LOAD(PFNGLUNIFORMMATRIX4FVPROC,         glUniformMatrix4fv);

#undef LOAD

    /* Verify a representative set of critical function pointers. */
    if (!glad_glClear          ||
        !glad_glCreateShader   ||
        !glad_glGenBuffers     ||
        !glad_glGenVertexArrays) {
        return 0;
    }
    return 1;
}
