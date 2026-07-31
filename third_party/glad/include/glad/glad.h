/*
 * Pre-generated GLAD loader for OpenGL 3.3 Core Profile
 * Based on GLAD 0.1.36 (https://glad.dav1d.de/)
 * Language: C, Specification: gl, API: gl=3.3, Profile: core
 *
 * Bundled with RenderLoopLab to avoid a Python3 build dependency.
 * Covers all GL 3.3 Core functions used by this project.
 */

#ifndef __glad_h_
#define __glad_h_

#ifdef __gl_h_
#error "OpenGL header already included; include glad/glad.h first"
#endif
#define __gl_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <KHR/khrplatform.h>

/* ========== Platform / calling-convention macros ========== */

#if defined(_WIN32) && !defined(APIENTRY)
#   define APIENTRY __stdcall
#endif
#ifndef APIENTRY
#   define APIENTRY
#endif
#ifndef APIENTRYP
#   define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#   define GLAPI extern
#endif

/* ========== GL base types ========== */

typedef unsigned int        GLenum;
typedef unsigned char       GLboolean;
typedef unsigned int        GLbitfield;
typedef signed   char       GLbyte;
typedef unsigned char       GLubyte;
typedef          short      GLshort;
typedef unsigned short      GLushort;
typedef          int        GLint;
typedef unsigned int        GLuint;
typedef          int        GLsizei;
typedef          float      GLfloat;
typedef          float      GLclampf;
typedef          double     GLdouble;
typedef          double     GLclampd;
typedef          char       GLchar;
typedef          void       GLvoid;
typedef khronos_intptr_t    GLintptr;
typedef khronos_ssize_t     GLsizeiptr;
typedef khronos_int64_t     GLint64;
typedef khronos_uint64_t    GLuint64;
typedef khronos_uint16_t    GLhalf;

/* ========== GL enums (1.0 – 3.3 Core) ========== */

/* Boolean */
#define GL_FALSE                          0
#define GL_TRUE                           1

/* Bitfield / clear */
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000

/* Primitives */
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006

/* Blending / factors */
#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305
#define GL_DST_COLOR                      0x0306
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_SRC_ALPHA_SATURATE             0x0308
#define GL_CONSTANT_COLOR                 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR       0x8002
#define GL_CONSTANT_ALPHA                 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA       0x8004
#define GL_BLEND_COLOR                    0x8005

/* Blend equations */
#define GL_FUNC_ADD                       0x8006
#define GL_FUNC_SUBTRACT                  0x800A
#define GL_FUNC_REVERSE_SUBTRACT          0x800B
#define GL_MIN                            0x8007
#define GL_MAX                            0x8008

/* Enable caps */
#define GL_BLEND                          0x0BE2
#define GL_DEPTH_TEST                     0x0B71
#define GL_STENCIL_TEST                   0x0B90
#define GL_SCISSOR_TEST                   0x0C11
#define GL_CULL_FACE                      0x0B44

/* Data types */
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_HALF_FLOAT                     0x140B
#define GL_DOUBLE                         0x140A
#define GL_FIXED                          0x140C
#define GL_INT_2_10_10_10_REV             0x8D9F
#define GL_UNSIGNED_INT_2_10_10_10_REV    0x8368

/* Texture targets */
#define GL_TEXTURE_2D                     0x0DE1
#define GL_TEXTURE_1D                     0x0DE0
#define GL_TEXTURE_3D                     0x806F

/* Texture parameters */
#define GL_TEXTURE_WIDTH                  0x1000
#define GL_TEXTURE_HEIGHT                 0x1001
#define GL_TEXTURE_INTERNAL_FORMAT        0x1003
#define GL_TEXTURE_BORDER_COLOR           0x1004
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_TEXTURE_WRAP_R                 0x8072

/* Texture filters */
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703

/* Texture wrap modes */
#define GL_REPEAT                         0x2901
#define GL_CLAMP_TO_BORDER                0x812D
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_MIRRORED_REPEAT                0x8370

/* Pixel formats */
#define GL_RED                            0x1903
#define GL_GREEN                          0x1904
#define GL_BLUE                           0x1905
#define GL_ALPHA                          0x1906
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_LUMINANCE                      0x1909
#define GL_LUMINANCE_ALPHA                0x190A
#define GL_BGR                            0x80E0
#define GL_BGRA                           0x80E1

/* Sized internal formats */
#define GL_R8                             0x8229
#define GL_RG8                            0x822B
#define GL_RGB8                           0x8051
#define GL_RGBA8                          0x8058
#define GL_R16F                           0x822D
#define GL_RG16F                          0x822F
#define GL_RGB16F                         0x881B
#define GL_RGBA16F                        0x881A
#define GL_RGBA32F                        0x8814

/* Pixel store parameters */
#define GL_UNPACK_SWAP_BYTES              0x0CF0
#define GL_UNPACK_LSB_FIRST               0x0CF1
#define GL_UNPACK_ROW_LENGTH              0x0CF2
#define GL_UNPACK_SKIP_ROWS               0x0CF3
#define GL_UNPACK_SKIP_PIXELS             0x0CF4
#define GL_UNPACK_ALIGNMENT               0x0CF5
#define GL_PACK_SWAP_BYTES                0x0D00
#define GL_PACK_LSB_FIRST                 0x0D01
#define GL_PACK_ROW_LENGTH                0x0D02
#define GL_PACK_SKIP_ROWS                 0x0D03
#define GL_PACK_SKIP_PIXELS               0x0D04
#define GL_PACK_ALIGNMENT                 0x0D05

/* Buffer targets */
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_PIXEL_PACK_BUFFER              0x88EB
#define GL_PIXEL_UNPACK_BUFFER            0x88EC
#define GL_UNIFORM_BUFFER                 0x8A11
#define GL_COPY_READ_BUFFER               0x8F36
#define GL_COPY_WRITE_BUFFER              0x8F37

/* Buffer usage hints */
#define GL_STREAM_DRAW                    0x88E0
#define GL_STREAM_READ                    0x88E1
#define GL_STREAM_COPY                    0x88E2
#define GL_STATIC_DRAW                    0x88E4
#define GL_STATIC_READ                    0x88E5
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_DYNAMIC_READ                   0x88E9
#define GL_DYNAMIC_COPY                   0x88EA

/* Shader types */
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_GEOMETRY_SHADER                0x8DD9

/* Shader parameters */
#define GL_SHADER_TYPE                    0x8B4F
#define GL_DELETE_STATUS                  0x8B80
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_ATTACHED_SHADERS               0x8B85
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define GL_ACTIVE_ATTRIBUTES              0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH    0x8B8A
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_CURRENT_PROGRAM                0x8B8D

/* String names */
#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03

/* Integer query names */
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_NUM_EXTENSIONS                 0x821D
#define GL_MAX_TEXTURE_SIZE               0x0D33
#define GL_MAX_VERTEX_ATTRIBS             0x8869
#define GL_MAX_TEXTURE_IMAGE_UNITS        0x8872
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D

/* Texture units */
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF

/* Errors */
#define GL_NO_ERROR                       0
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_STACK_OVERFLOW                 0x0503
#define GL_STACK_UNDERFLOW                0x0504
#define GL_OUT_OF_MEMORY                  0x0505
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506

/* Framebuffer */
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER                    0x8D40
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_RENDERBUFFER                   0x8D41

/* ========== Function pointer typedefs ========== */

/* 1.0 / 1.1 functions available as proper function pointers */
typedef void       (APIENTRYP PFNGLVIEWPORTPROC)         (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void       (APIENTRYP PFNGLCLEARPROC)             (GLbitfield mask);
typedef void       (APIENTRYP PFNGLCLEARCOLORPROC)        (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void       (APIENTRYP PFNGLENABLEPROC)            (GLenum cap);
typedef void       (APIENTRYP PFNGLDISABLEPROC)           (GLenum cap);
typedef void       (APIENTRYP PFNGLBLENDFUNCPROC)         (GLenum sfactor, GLenum dfactor);
typedef void       (APIENTRYP PFNGLBLENDEQUATIONPROC)     (GLenum mode);
typedef void       (APIENTRYP PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void       (APIENTRYP PFNGLSCISSORPROC)           (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void       (APIENTRYP PFNGLPIXELSTOREIPROC)       (GLenum pname, GLint param);
typedef void       (APIENTRYP PFNGLDRAWELEMENTSPROC)      (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
typedef void       (APIENTRYP PFNGLDRAWARRAYSPROC)        (GLenum mode, GLint first, GLsizei count);
typedef const GLubyte* (APIENTRYP PFNGLGETSTRINGPROC)     (GLenum name);
typedef const GLubyte* (APIENTRYP PFNGLGETSTRINGIPROC)    (GLenum name, GLuint index);
typedef void       (APIENTRYP PFNGLGETINTEGERVPROC)       (GLenum pname, GLint* data);
typedef GLenum     (APIENTRYP PFNGLGETERRORPROC)          (void);

/* Textures */
typedef void  (APIENTRYP PFNGLGENTEXTURESPROC)      (GLsizei n, GLuint* textures);
typedef void  (APIENTRYP PFNGLDELETETEXTURESPROC)   (GLsizei n, const GLuint* textures);
typedef void  (APIENTRYP PFNGLBINDTEXTUREPROC)      (GLenum target, GLuint texture);
typedef void  (APIENTRYP PFNGLTEXIMAGE2DPROC)       (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
typedef void  (APIENTRYP PFNGLTEXSUBIMAGE2DPROC)    (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);
typedef void  (APIENTRYP PFNGLTEXPARAMETERIPROC)    (GLenum target, GLenum pname, GLint param);
typedef void  (APIENTRYP PFNGLTEXPARAMETERFPROC)    (GLenum target, GLenum pname, GLfloat param);
typedef void  (APIENTRYP PFNGLGENERATEMIPMAPPROC)   (GLenum target);
typedef void  (APIENTRYP PFNGLACTIVETEXTUREPROC)    (GLenum texture);

/* Buffers (1.5) */
typedef void  (APIENTRYP PFNGLGENBUFFERSPROC)       (GLsizei n, GLuint* buffers);
typedef void  (APIENTRYP PFNGLDELETEBUFFERSPROC)    (GLsizei n, const GLuint* buffers);
typedef void  (APIENTRYP PFNGLBINDBUFFERPROC)       (GLenum target, GLuint buffer);
typedef void  (APIENTRYP PFNGLBUFFERDATAPROC)       (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
typedef void  (APIENTRYP PFNGLBUFFERSUBDATAPROC)    (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
typedef void* (APIENTRYP PFNGLMAPBUFFERPROC)        (GLenum target, GLenum access);
typedef GLboolean (APIENTRYP PFNGLUNMAPBUFFERPROC)  (GLenum target);

/* Vertex arrays (3.0 Core) */
typedef void  (APIENTRYP PFNGLGENVERTEXARRAYSPROC)         (GLsizei n, GLuint* arrays);
typedef void  (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC)      (GLsizei n, const GLuint* arrays);
typedef void  (APIENTRYP PFNGLBINDVERTEXARRAYPROC)         (GLuint array);
typedef void  (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void  (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void  (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)     (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
typedef void  (APIENTRYP PFNGLVERTEXATTRIBIPOINTERPROC)    (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
typedef void  (APIENTRYP PFNGLVERTEXATTRIBDIVISORPROC)     (GLuint index, GLuint divisor);
typedef void  (APIENTRYP PFNGLDRAWELEMENTSINSTANCEDPROC)   (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, GLsizei instancecount);
typedef void  (APIENTRYP PFNGLDRAWARRAYSINSTANCEDPROC)     (GLenum mode, GLint first, GLsizei count, GLsizei instancecount);

/* Shaders (2.0) */
typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC)     (GLenum type);
typedef void   (APIENTRYP PFNGLDELETESHADERPROC)     (GLuint shader);
typedef void   (APIENTRYP PFNGLSHADERSOURCEPROC)     (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
typedef void   (APIENTRYP PFNGLCOMPILESHADERPROC)    (GLuint shader);
typedef void   (APIENTRYP PFNGLGETSHADERIVPROC)      (GLuint shader, GLenum pname, GLint* params);
typedef void   (APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);

/* Programs (2.0) */
typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC)     (void);
typedef void   (APIENTRYP PFNGLDELETEPROGRAMPROC)     (GLuint program);
typedef void   (APIENTRYP PFNGLATTACHSHADERPROC)      (GLuint program, GLuint shader);
typedef void   (APIENTRYP PFNGLDETACHSHADERPROC)      (GLuint program, GLuint shader);
typedef void   (APIENTRYP PFNGLLINKPROGRAMPROC)       (GLuint program);
typedef void   (APIENTRYP PFNGLGETPROGRAMIVPROC)      (GLuint program, GLenum pname, GLint* params);
typedef void   (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void   (APIENTRYP PFNGLUSEPROGRAMPROC)        (GLuint program);

/* Uniforms (2.0) */
typedef GLint  (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC)  (GLuint program, const GLchar* name);
typedef void   (APIENTRYP PFNGLUNIFORM1IPROC)           (GLint location, GLint v0);
typedef void   (APIENTRYP PFNGLUNIFORM2IPROC)           (GLint location, GLint v0, GLint v1);
typedef void   (APIENTRYP PFNGLUNIFORM3IPROC)           (GLint location, GLint v0, GLint v1, GLint v2);
typedef void   (APIENTRYP PFNGLUNIFORM4IPROC)           (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void   (APIENTRYP PFNGLUNIFORM1FPROC)           (GLint location, GLfloat v0);
typedef void   (APIENTRYP PFNGLUNIFORM2FPROC)           (GLint location, GLfloat v0, GLfloat v1);
typedef void   (APIENTRYP PFNGLUNIFORM3FPROC)           (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void   (APIENTRYP PFNGLUNIFORM4FPROC)           (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void   (APIENTRYP PFNGLUNIFORM1IVPROC)          (GLint location, GLsizei count, const GLint* value);
typedef void   (APIENTRYP PFNGLUNIFORM2IVPROC)          (GLint location, GLsizei count, const GLint* value);
typedef void   (APIENTRYP PFNGLUNIFORM3IVPROC)          (GLint location, GLsizei count, const GLint* value);
typedef void   (APIENTRYP PFNGLUNIFORM4IVPROC)          (GLint location, GLsizei count, const GLint* value);
typedef void   (APIENTRYP PFNGLUNIFORM1FVPROC)          (GLint location, GLsizei count, const GLfloat* value);
typedef void   (APIENTRYP PFNGLUNIFORM2FVPROC)          (GLint location, GLsizei count, const GLfloat* value);
typedef void   (APIENTRYP PFNGLUNIFORM3FVPROC)          (GLint location, GLsizei count, const GLfloat* value);
typedef void   (APIENTRYP PFNGLUNIFORM4FVPROC)          (GLint location, GLsizei count, const GLfloat* value);
typedef void   (APIENTRYP PFNGLUNIFORMMATRIX2FVPROC)    (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void   (APIENTRYP PFNGLUNIFORMMATRIX3FVPROC)    (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void   (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC)    (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

/* ========== Extern function pointers ========== */

extern PFNGLVIEWPORTPROC                  glad_glViewport;
extern PFNGLCLEARPROC                     glad_glClear;
extern PFNGLCLEARCOLORPROC                glad_glClearColor;
extern PFNGLENABLEPROC                    glad_glEnable;
extern PFNGLDISABLEPROC                   glad_glDisable;
extern PFNGLBLENDFUNCPROC                 glad_glBlendFunc;
extern PFNGLBLENDEQUATIONPROC             glad_glBlendEquation;
extern PFNGLBLENDFUNCSEPARATEPROC         glad_glBlendFuncSeparate;
extern PFNGLSCISSORPROC                   glad_glScissor;
extern PFNGLPIXELSTOREIPROC               glad_glPixelStorei;
extern PFNGLDRAWELEMENTSPROC              glad_glDrawElements;
extern PFNGLDRAWARRAYSPROC                glad_glDrawArrays;
extern PFNGLGETSTRINGPROC                 glad_glGetString;
extern PFNGLGETSTRINGIPROC                glad_glGetStringi;
extern PFNGLGETINTEGERVPROC               glad_glGetIntegerv;
extern PFNGLGETERRORPROC                  glad_glGetError;

extern PFNGLGENTEXTURESPROC               glad_glGenTextures;
extern PFNGLDELETETEXTURESPROC            glad_glDeleteTextures;
extern PFNGLBINDTEXTUREPROC               glad_glBindTexture;
extern PFNGLTEXIMAGE2DPROC                glad_glTexImage2D;
extern PFNGLTEXSUBIMAGE2DPROC             glad_glTexSubImage2D;
extern PFNGLTEXPARAMETERIPROC             glad_glTexParameteri;
extern PFNGLTEXPARAMETERFPROC             glad_glTexParameterf;
extern PFNGLGENERATEMIPMAPPROC            glad_glGenerateMipmap;
extern PFNGLACTIVETEXTUREPROC             glad_glActiveTexture;

extern PFNGLGENBUFFERSPROC                glad_glGenBuffers;
extern PFNGLDELETEBUFFERSPROC             glad_glDeleteBuffers;
extern PFNGLBINDBUFFERPROC                glad_glBindBuffer;
extern PFNGLBUFFERDATAPROC                glad_glBufferData;
extern PFNGLBUFFERSUBDATAPROC             glad_glBufferSubData;
extern PFNGLMAPBUFFERPROC                 glad_glMapBuffer;
extern PFNGLUNMAPBUFFERPROC               glad_glUnmapBuffer;

extern PFNGLGENVERTEXARRAYSPROC           glad_glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC        glad_glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC           glad_glBindVertexArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC   glad_glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC  glad_glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC       glad_glVertexAttribPointer;
extern PFNGLVERTEXATTRIBIPOINTERPROC      glad_glVertexAttribIPointer;
extern PFNGLVERTEXATTRIBDIVISORPROC       glad_glVertexAttribDivisor;
extern PFNGLDRAWELEMENTSINSTANCEDPROC     glad_glDrawElementsInstanced;
extern PFNGLDRAWARRAYSINSTANCEDPROC       glad_glDrawArraysInstanced;

extern PFNGLCREATESHADERPROC              glad_glCreateShader;
extern PFNGLDELETESHADERPROC              glad_glDeleteShader;
extern PFNGLSHADERSOURCEPROC              glad_glShaderSource;
extern PFNGLCOMPILESHADERPROC             glad_glCompileShader;
extern PFNGLGETSHADERIVPROC               glad_glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC          glad_glGetShaderInfoLog;

extern PFNGLCREATEPROGRAMPROC             glad_glCreateProgram;
extern PFNGLDELETEPROGRAMPROC             glad_glDeleteProgram;
extern PFNGLATTACHSHADERPROC              glad_glAttachShader;
extern PFNGLDETACHSHADERPROC              glad_glDetachShader;
extern PFNGLLINKPROGRAMPROC               glad_glLinkProgram;
extern PFNGLGETPROGRAMIVPROC              glad_glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC         glad_glGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC                glad_glUseProgram;

extern PFNGLGETUNIFORMLOCATIONPROC        glad_glGetUniformLocation;
extern PFNGLUNIFORM1IPROC                 glad_glUniform1i;
extern PFNGLUNIFORM2IPROC                 glad_glUniform2i;
extern PFNGLUNIFORM3IPROC                 glad_glUniform3i;
extern PFNGLUNIFORM4IPROC                 glad_glUniform4i;
extern PFNGLUNIFORM1FPROC                 glad_glUniform1f;
extern PFNGLUNIFORM2FPROC                 glad_glUniform2f;
extern PFNGLUNIFORM3FPROC                 glad_glUniform3f;
extern PFNGLUNIFORM4FPROC                 glad_glUniform4f;
extern PFNGLUNIFORM1IVPROC                glad_glUniform1iv;
extern PFNGLUNIFORM2IVPROC                glad_glUniform2iv;
extern PFNGLUNIFORM3IVPROC                glad_glUniform3iv;
extern PFNGLUNIFORM4IVPROC                glad_glUniform4iv;
extern PFNGLUNIFORM1FVPROC                glad_glUniform1fv;
extern PFNGLUNIFORM2FVPROC                glad_glUniform2fv;
extern PFNGLUNIFORM3FVPROC                glad_glUniform3fv;
extern PFNGLUNIFORM4FVPROC                glad_glUniform4fv;
extern PFNGLUNIFORMMATRIX2FVPROC          glad_glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC          glad_glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC          glad_glUniformMatrix4fv;

/* ========== Convenience #defines (standard GLAD v1 style) ========== */

#define glViewport                  glad_glViewport
#define glClear                     glad_glClear
#define glClearColor                glad_glClearColor
#define glEnable                    glad_glEnable
#define glDisable                   glad_glDisable
#define glBlendFunc                 glad_glBlendFunc
#define glBlendEquation             glad_glBlendEquation
#define glBlendFuncSeparate         glad_glBlendFuncSeparate
#define glScissor                   glad_glScissor
#define glPixelStorei               glad_glPixelStorei
#define glDrawElements              glad_glDrawElements
#define glDrawArrays                glad_glDrawArrays
#define glGetString                 glad_glGetString
#define glGetStringi                glad_glGetStringi
#define glGetIntegerv               glad_glGetIntegerv
#define glGetError                  glad_glGetError

#define glGenTextures               glad_glGenTextures
#define glDeleteTextures            glad_glDeleteTextures
#define glBindTexture               glad_glBindTexture
#define glTexImage2D                glad_glTexImage2D
#define glTexSubImage2D             glad_glTexSubImage2D
#define glTexParameteri             glad_glTexParameteri
#define glTexParameterf             glad_glTexParameterf
#define glGenerateMipmap            glad_glGenerateMipmap
#define glActiveTexture             glad_glActiveTexture

#define glGenBuffers                glad_glGenBuffers
#define glDeleteBuffers             glad_glDeleteBuffers
#define glBindBuffer                glad_glBindBuffer
#define glBufferData                glad_glBufferData
#define glBufferSubData             glad_glBufferSubData
#define glMapBuffer                 glad_glMapBuffer
#define glUnmapBuffer               glad_glUnmapBuffer

#define glGenVertexArrays           glad_glGenVertexArrays
#define glDeleteVertexArrays        glad_glDeleteVertexArrays
#define glBindVertexArray           glad_glBindVertexArray
#define glEnableVertexAttribArray   glad_glEnableVertexAttribArray
#define glDisableVertexAttribArray  glad_glDisableVertexAttribArray
#define glVertexAttribPointer       glad_glVertexAttribPointer
#define glVertexAttribIPointer      glad_glVertexAttribIPointer
#define glVertexAttribDivisor       glad_glVertexAttribDivisor
#define glDrawElementsInstanced     glad_glDrawElementsInstanced
#define glDrawArraysInstanced       glad_glDrawArraysInstanced

#define glCreateShader              glad_glCreateShader
#define glDeleteShader              glad_glDeleteShader
#define glShaderSource              glad_glShaderSource
#define glCompileShader             glad_glCompileShader
#define glGetShaderiv               glad_glGetShaderiv
#define glGetShaderInfoLog          glad_glGetShaderInfoLog

#define glCreateProgram             glad_glCreateProgram
#define glDeleteProgram             glad_glDeleteProgram
#define glAttachShader              glad_glAttachShader
#define glDetachShader              glad_glDetachShader
#define glLinkProgram               glad_glLinkProgram
#define glGetProgramiv              glad_glGetProgramiv
#define glGetProgramInfoLog         glad_glGetProgramInfoLog
#define glUseProgram                glad_glUseProgram

#define glGetUniformLocation        glad_glGetUniformLocation
#define glUniform1i                 glad_glUniform1i
#define glUniform2i                 glad_glUniform2i
#define glUniform3i                 glad_glUniform3i
#define glUniform4i                 glad_glUniform4i
#define glUniform1f                 glad_glUniform1f
#define glUniform2f                 glad_glUniform2f
#define glUniform3f                 glad_glUniform3f
#define glUniform4f                 glad_glUniform4f
#define glUniform1iv                glad_glUniform1iv
#define glUniform2iv                glad_glUniform2iv
#define glUniform3iv                glad_glUniform3iv
#define glUniform4iv                glad_glUniform4iv
#define glUniform1fv                glad_glUniform1fv
#define glUniform2fv                glad_glUniform2fv
#define glUniform3fv                glad_glUniform3fv
#define glUniform4fv                glad_glUniform4fv
#define glUniformMatrix2fv          glad_glUniformMatrix2fv
#define glUniformMatrix3fv          glad_glUniformMatrix3fv
#define glUniformMatrix4fv          glad_glUniformMatrix4fv

/* ========== GLAD API ========== */

typedef void* (*GLADloadproc)(const char* name);

/**
 * Load all GL 3.3 Core function pointers using the provided loader.
 * Returns 1 on success, 0 if any critical function pointer is NULL.
 *
 * Typical usage with SDL2:
 *   gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
 */
int gladLoadGLLoader(GLADloadproc load);

#ifdef __cplusplus
}
#endif

#endif /* __glad_h_ */
