/*
** Copyright (c) 2008-2018 The Khronos Group Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
**
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/

/* Khronos platform-specific types and definitions.
 *
 * The master copy of khrplatform.h is maintained in the Khronos EGL
 * Registry repository at https://github.com/KhronosGroup/EGL-Registry
 * The last semantic modification to khrplatform.h was at commit ID:
 *      67a3e0864c2d75ea5287b9f3d2eb74a745936692
 */

#ifndef __khrplatform_h_
#define __khrplatform_h_

/*
 * Khronos type definitions -- requires compiler support for 'signed'
 * and 'unsigned' in addition to the types defined in <stdint.h>
 * and the C++ bool type.
 */

#include <stdint.h>

#ifndef KHRONOS_APICALL
#   if defined(_WIN32) && !defined(__SCITECH_SNAP__)
#       define KHRONOS_APICALL __declspec(dllimport)
#   elif defined (__SYMBIAN32__)
#       define KHRONOS_APICALL IMPORT_C
#   elif defined(__ANDROID__)
#       define KHRONOS_APICALL __attribute__((visibility("default")))
#   else
#       define KHRONOS_APICALL
#   endif
#endif

#ifndef KHRONOS_APIENTRY
#   if defined(_WIN32) && !defined(_WIN32_WCE) && !defined(__SCITECH_SNAP__)
#       define KHRONOS_APIENTRY __stdcall
#   else
#       define KHRONOS_APIENTRY
#   endif
#endif

#if !defined(KHRONOS_SUPPORT_INT64)
#   define KHRONOS_SUPPORT_INT64   1
#endif
#if !defined(KHRONOS_SUPPORT_FLOAT)
#   define KHRONOS_SUPPORT_FLOAT   1
#endif

/*
 * To enable function prototypes when compiling with GL_GLEXT_PROTOTYPES:
 */
#ifndef KHRONOS_APIATTRIBUTES
#   define KHRONOS_APIATTRIBUTES
#endif

/*-------------------------------------------------------------------------
 * Definition of khronos_int8_t, khronos_uint8_t, khronos_int16_t,
 * khronos_uint16_t, khronos_int32_t, khronos_uint32_t, khronos_int64_t,
 * khronos_uint64_t, khronos_intptr_t, khronos_uintptr_t,
 * khronos_ssize_t, khronos_usize_t and khronos_float_t
 *-----------------------------------------------------------------------*/

/* Khronos integer types */
typedef          int8_t   khronos_int8_t;
typedef         uint8_t   khronos_uint8_t;
typedef         int16_t   khronos_int16_t;
typedef        uint16_t   khronos_uint16_t;
typedef         int32_t   khronos_int32_t;
typedef        uint32_t   khronos_uint32_t;
typedef         int64_t   khronos_int64_t;
typedef        uint64_t   khronos_uint64_t;

#define KHRONOS_MAX_ENUM 0x7FFFFFFF /* Largest enumeration value */

/* Khronos boolean */
typedef unsigned int  khronos_boolean_enum_t;
#define KHRONOS_FALSE 0
#define KHRONOS_TRUE  1

/*
 * Pointer-sized signed/unsigned integers
 */
typedef intptr_t  khronos_intptr_t;
typedef uintptr_t khronos_uintptr_t;

/*
 * khronos_ssize_t  is a signed integer type capable of representing
 *     the largest number of bytes that can be allocated by a single
 *     contiguous memory allocation.
 *
 * khronos_usize_t  is an unsigned integer type capable of representing
 *     the largest number of bytes that can be allocated by a single
 *     contiguous memory allocation.
 */
#if defined(_WIN64)
    typedef signed   long long int khronos_ssize_t;
    typedef unsigned long long int khronos_usize_t;
#else
    typedef signed   long  int     khronos_ssize_t;
    typedef unsigned long  int     khronos_usize_t;
#endif

/*
 * khronos_float_t is a 32-bit floating point number
 */
typedef          float   khronos_float_t;

#endif /* __khrplatform_h_ */
