#pragma once

#include <stdint.h>

#ifndef __cplusplus

#include <stdbool.h>

#endif

#ifndef SAVANNA_CORE_PRIMITIVES
#define SAVANNA_CORE_PRIMITIVES

// Global namespace typedefs
typedef bool                se_bool;
typedef char                se_byte;
typedef unsigned char       se_ubyte;

typedef uint8_t             se_uint8;
typedef uint16_t            se_uint16;
typedef uint32_t            se_uint32;
typedef uint64_t            se_uint64;
typedef int8_t              se_int8;
typedef int16_t             se_int16;
typedef int32_t             se_int32;
typedef int64_t             se_int64;
typedef intptr_t            se_intptr;
typedef uintptr_t           se_uintptr;

typedef size_t              se_size;

typedef se_int16            se_half_word;
typedef se_uint16           se_uhalf_word;
typedef se_int32            se_word;
typedef se_uint32           se_uword;
typedef se_int64            se_double_word;
typedef se_uint64           se_udouble_word;

// Union quadword
typedef union se_quadword
{
    se_byte         bytes[16];
    se_ubyte        ubytes[16];

    se_half_word    halfwords[8];
    se_uhalf_word   uhalfwords[8];

    se_word         words[4];
    se_uword        uwords[4];

    se_double_word  dwords[2];
    se_udouble_word udwords[2];
} se_quadword;

#if __cplusplus
namespace Savanna
{
    using byte = se_byte;
    using ubyte = se_ubyte;

    using uint8 = se_uint8;
    using uint16 = se_uint16;
    using uint32 = se_uint32;
    using uint64 = se_uint64;
    using int8 = se_int8;
    using int16 = se_int16;
    using int32 = se_int32;
    using int64 = se_int64;

    using half_word = se_half_word;
    using uhalf_word = se_uhalf_word;

    using word = se_word;
    using uword = se_uword;

    using double_word = se_double_word;
    using udouble_word = se_udouble_word;

    using quad_word = se_quadword;
    using uquad_word = se_quadword;

    using intptr = se_intptr;
    using uintptr = se_uintptr;

    using size = se_size;
}
#endif // !__cplusplus

#endif // !SAVANNA_CORE_PRIMITIVES

// #ifndef SAVANNA_INTERFACE_DEFINITIONS_H
// #define SAVANNA_INTERFACE_DEFINITIONS_H

// #ifdef __cplusplus
// #   define SAVANNA_EXTERN extern "C"
// #else // !__cplusplus
// #   define SAVANNA_EXTERN
// #endif // __cplusplus

// #ifdef SAVANNA_EXPORTS
// #   undef SAVANNA_EXPORTS
// #   define SAVANNA_EXPORTS 1
// #   define SAVANNA_DECL_SPEC_OPTION dllexport
// #else // !SAVANNA_EXPORTS
// #   define SAVANNA_EXPORTS 0
// #   define SAVANNA_DECL_SPEC_OPTION dllimport
// #endif // SAVANNA_EXPORTS

// // Savanna Interface API
// #if SAVANNA_WINDOWS
// #   define SAVANNA_INTERFACE_API __stdcall
// #   define SAVANNA_INTERFACE __declspec(SAVANNA_DECL_SPEC_OPTION)
// #elif SAVANNA_MACOS // !SAVANNA_WINDOWS
// #   define SAVANNA_INTERFACE_API
// #   define SAVANNA_INTERFACE
// #elif SAVANNA_LINUX // !SAVANNA_WINDOWS && !SAVANNA_MACOS
// #   define SAVANNA_INTERFACE_API
// #   define SAVANNA_INTERFACE
// #else // !SAVANNA_WINDOWS && !SAVANNA_MACOS && !SAVANNA_LINUX
// #   error "Unsupported platform."
// #endif // SAVANNA_WINDOWS

// #if SAVANNA_EXPORTS
// #   if SAVANNA_WINDOWS
// #       define SAVANNA_EXPORT(ret) SAVANNA_EXTERN __declspec(dllexport) ret SAVANNA_INTERFACE_API
// #   else // !SAVANNA_WINDOWS
// #       define SAVANNA_EXPORT(ret) SAVANNA_EXTERN ret SAVANNA_INTERFACE_API
// #   endif // SAVANNA_WINDOWS
// #else // SAVANNA_EXPORTS
// #   define SAVANNA_EXPORT(ret) SAVANNA_EXTERN ret SAVANNA_INTERFACE_API
// #endif // SAVANNA_EXPORTS

// #endif // !SAVANNA_INTERFACE_DEFINITIONS_H

// #endif // !SAVANNA_INTERFACE_DEFINITIONS_H

typedef se_int64 se_InterfaceHandle_t;

typedef struct ISavannaInterface
{
    void (*Log)(const char* pMessage);
} ISavannaInterface;

typedef struct ISavannaPluginLifecycle
{
    void (*Initialize)(ISavannaInterface const* pInterface);
    void (*Deinitialize)();
} ISavannaLifecycle;

/**
 * @brief Registers a plugin interface with the Savanna engine. This function is called by the plugin
 *       during it's own initialization.
*/
// SAVANNA_EXPORT(se_InterfaceHandle_t) Savanna_RegisterPluginInterface(const ISavannaPluginLifecycle* pInterface);

// SAVANNA_EXPORT(const void*) Savanna_GetPluginInterface(const se_InterfaceHandle_t handle);
