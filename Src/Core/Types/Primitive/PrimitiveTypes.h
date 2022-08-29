#pragma once

#include <stdint.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif

// Global namespace typedefs
typedef bool                __se_bool;
typedef char                __se_byte;
typedef unsigned char       __se_ubyte;

typedef uint8_t             __se_uint8;
typedef uint16_t            __se_uint16;
typedef uint32_t            __se_uint32;
typedef uint64_t            __se_uint64;
typedef int8_t              __se_int8;
typedef int16_t             __se_int16;
typedef int32_t             __se_int32;
typedef int64_t             __se_int64;
typedef intptr_t            __se_intptr;
typedef uintptr_t           __se_uintptr;

typedef __se_byte           s_byte;
typedef __se_ubyte          s_ubyte;

typedef __se_uint8          s_uint8;
typedef __se_uint16         s_uint16;
typedef __se_uint32         s_uint32;
typedef __se_uint64         s_uint64;
typedef __se_int8           s_int8;
typedef __se_int16          s_int16;
typedef __se_int32          s_int32;
typedef __se_int64          s_int64;

typedef __se_intptr         s_intptr;
typedef __se_uintptr        s_uintptr;

#if __cplusplus
namespace Savanna
{
    // Namespaced typedefs
    typedef __se_byte       byte;
    typedef __se_ubyte      ubyte;

    typedef __se_uint8      uint8;
    typedef __se_uint16     uint16;
    typedef __se_uint32     uint32;
    typedef __se_uint64     uint64;
    typedef __se_int8       int8;
    typedef __se_int16      int16;
    typedef __se_int32      int32;
    typedef __se_int64      int64;

    typedef __se_intptr     intptr;
    typedef __se_uintptr    uintptr;
}
#endif
