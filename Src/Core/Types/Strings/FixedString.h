#pragma once

#if __cplusplus

#include <cstring>
#include <cassert>
#include <string>

#else

#include <string.h>
#include <assert.h>

#endif

#include "Types/SavannaClass.h"

#include "Memory/CacheLine.h"

#include "Memory/CacheLine.h"

#if __cplusplus

#define DECLARE_SAVANNA_CPP_FIXED_STRING_WITH_LENGTH(__strlen__) \
namespace Savanna \
{ \
    class FixedString##__strlen__ \
    { \
    public: \
        FixedString##__strlen__(const char* cstring, size_t start, size_t size) \
            : m_StringLength(0)\
        { \
            SAVANNA_INSERT_SCOPED_PROFILER("FixedString" #__strlen__ "::ctor");\
            memset(m_Bytes, 0, __strlen__); \
            if (cstring != nullptr && start < size) \
            { \
                CopyFrom(cstring, start, size);\
            } \
        } \
\
        FixedString##__strlen__(const char* cstring = "") \
            : FixedString##__strlen__(cstring, 0, strlen(cstring) + 1) \
        {} \
\
        FixedString##__strlen__(const FixedString##__strlen__ &other) \
            : FixedString##__strlen__(other.m_Characters, 0, __strlen__) \
        {} \
\
        FixedString##__strlen__(std::string const str) \
            : FixedString##__strlen__(str.c_str(), 0, str.length()) \
        {} \
\
        void Append(std::string const str, size_t start = 0)\
        {\
            CopyFrom(str.c_str(), start, str.length());\
        }\
\
        void Append(const char* cstring, size_t start = 0)\
        {\
            CopyFrom(cstring, start, strlen(cstring));\
        }\
\
        void Append(const char* cstring, size_t start, size_t size)\
        {\
            CopyFrom(cstring, start, size);\
        }\
\
        const char* c_str() const { return m_Characters; } \
\
        FixedString##__strlen__& operator=(const FixedString##__strlen__& other) \
        { \
            SAVANNA_INSERT_SCOPED_PROFILER("FixedString" #__strlen__ "::operator=");\
            if (this == &other) \
                return *this; \
\
            memcpy(m_Bytes, other.m_Bytes, __strlen__); \
            return *this; \
        } \
\
        template<typename TPrimitive> FixedString##__strlen__(const char* format, TPrimitive primitive) \
        { \
            sprintf(m_Characters, format, primitive); \
        }\
\
        operator const char *() const \
        { \
            return m_Characters; \
        } \
\
        size_t operator()(const FixedString##__strlen__ &str) const \
        { \
            size_t result = 2166136261; \
            for (size_t i = 0; i < __strlen__; ++i) \
            { \
                result = (result * 16777619) ^ str.m_Characters[i]; \
            } \
            return result ^ (__strlen__ << 1); \
        } \
\
    private:\
        inline void CopyFrom(const char* cstring, size_t start, size_t size, size_t destOffset = 0) \
        {\
            size_t copyLength = size - start > size - 1 ? size - 1 : size - start; \
            assert(copyLength < __strlen__ - m_StringLength - destOffset); \
            memcpy(m_Bytes + destOffset, cstring, copyLength); \
            m_StringLength += copyLength;\
        }\
\
        union \
        { \
            SAVANNA_CACHELINE_ALIGN __se_FixedString##__strlen__ m_CFixedString; \
            SAVANNA_CACHELINE_ALIGN char m_Characters[ __strlen__ ]; \
            SAVANNA_CACHELINE_ALIGN __se_byte m_Bytes[ __strlen__ ]; \
        }; \
    private:\
        SAVANNA_CACHELINE_ALIGN size_t m_StringLength; \
    }; \
} // namespace Savanna

#else
#define DECLARE_SAVANNA_CPP_FIXED_STRING(__strlen__)
#endif

#define DECLARE_SAVANNA_FIXED_STRING_WITH_LENGTH(__strlen__) \
    typedef struct SAVANNA_CACHELINE_ALIGN \
    { \
        union \
        { \
            SAVANNA_CACHELINE_ALIGN char characters[ __strlen__ ]; \
            SAVANNA_CACHELINE_ALIGN __se_byte bytes[ __strlen__ ]; \
        }; \
    } __se_FixedString##__strlen__; \
    DECLARE_SAVANNA_CPP_FIXED_STRING_WITH_LENGTH(__strlen__);

// Declarations

DECLARE_SAVANNA_FIXED_STRING_WITH_LENGTH(8);
DECLARE_SAVANNA_FIXED_STRING_WITH_LENGTH(16);
DECLARE_SAVANNA_FIXED_STRING_WITH_LENGTH(32);
DECLARE_SAVANNA_FIXED_STRING_WITH_LENGTH(64);
DECLARE_SAVANNA_FIXED_STRING_WITH_LENGTH(128);
DECLARE_SAVANNA_FIXED_STRING_WITH_LENGTH(256);
DECLARE_SAVANNA_FIXED_STRING_WITH_LENGTH(512);
DECLARE_SAVANNA_FIXED_STRING_WITH_LENGTH(1024);
DECLARE_SAVANNA_FIXED_STRING_WITH_LENGTH(2048);
DECLARE_SAVANNA_FIXED_STRING_WITH_LENGTH(4096);

#undef DECLARE_SAVANNA_FIXED_STRING_WITH_LENGTH
#undef DECLARE_SAVANNA_CPP_FIXED_STRING_WITH_LENGTH
