/**
 * @file Pages.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Provides definitions for variable sized memory block structs in ranging KiB sizes.
 * These structs are useful for chunking views of contiguous memory into specifically sized chunks.
 *
 * @version 0.1
 * @date 2022-08-02
 *
 */
#pragma once

#include <memory.h>

#include "CacheLine.h"
#include "Math/MathHelpers.h"
#include "Public/ISavannaEngine.h"

#include "Utilities/SavannaCoding.h"

/**
 * @brief TODO @David.Mohrhardt Document
 */
#define DECLARE_SAVANNA_KIB_PAGE(__kibLength) \
    typedef struct alignas( L1CacheLineLength() ) sePage##__kibLength##KiB \
    { \
        union \
        { \
            alignas( L1CacheLineLength() ) se_byte m_##__kibLength##Block[KibiBytesToBytes(__kibLength)]; \
            alignas( L1CacheLineLength() ) se_L1CacheLine m_##__kibLength##CacheLines[ GetL1CacheLineCount( KibiBytesToBytes(__kibLength) ) ]; \
        }; \
    } sePage##__kibLength##KiB; \
    namespace savanna { using Page##__kibLength##KiB = sePage##__kibLength##KiB; }

/**
 * @brief returns the conversion from KiB to B.
 *
 * @param KiBLength the length in KiB.
 * @return constexpr size_t size in Bytes.
 */
static consteval size_t KibiBytesToBytes(size_t KiBLength)
{
    return KiBLength * 1024;
}

/**
 * @brief Declare a memory block union of size 1KiB.
 */
DECLARE_SAVANNA_KIB_PAGE(1);

/**
 * @brief Declare a memory block union of size 2KiB.
 */
DECLARE_SAVANNA_KIB_PAGE(2);

/**
 * @brief Declare a memory block union of size 4KiB.
 */
DECLARE_SAVANNA_KIB_PAGE(4);

/**
 * @brief Declare a memory block union of size 8KiB.
 */
DECLARE_SAVANNA_KIB_PAGE(8);

/**
 * @brief Declare a memory block union of size 16KiB.
 */
DECLARE_SAVANNA_KIB_PAGE(16);

/**
 * @brief Declare a memory block union of size 32KiB.
 */
DECLARE_SAVANNA_KIB_PAGE(32);

/**
 * @brief Declare a memory block union of size 64KiB.
 */
DECLARE_SAVANNA_KIB_PAGE(64);

/**
 * @brief Declare a memory block union of size 128KiB.
 */
DECLARE_SAVANNA_KIB_PAGE(128);

/**
 * @brief Declare a memory block union of size 256KiB.
 */
DECLARE_SAVANNA_KIB_PAGE(256);

/**
 * @brief Declare a memory block union of size 512KiB.
 */
DECLARE_SAVANNA_KIB_PAGE(512);

/**
 * @brief Declare a memory block union of size 1024KiB.
 */
DECLARE_SAVANNA_KIB_PAGE(1024);

/**
 * @brief Declare a memory block union of size 2048KiB.
 */
DECLARE_SAVANNA_KIB_PAGE(2048);

/**
 * @brief Declare a memory block union of size 4096KiB.
 */
DECLARE_SAVANNA_KIB_PAGE(4096);

/**
 * @brief TODO @David.Mohrhardt Document
 */
#define DECLARE_UNIFIED_PAGE_STRUCT(__memoryBlockSize, __unionMembers) \
    typedef struct seUnifiedPage##__memoryBlockSize##KiB \
    { \
        const static size_t k_BlockSize = __memoryBlockSize; \
        union \
        { \
            sePage##__memoryBlockSize##KiB m_##__memoryBlockSize##KiBBlock; \
            __unionMembers \
        }; \
    } seUnifiedPage##__memoryBlockSize##KiB; \
    namespace savanna { using UnifiedPage##__memoryBlockSize##KiB = seUnifiedPage##__memoryBlockSize##KiB; }

/**
 * @brief Creates an array of se_UnifiedPages that will fit to the size of the union of the given integer size in KiB.
 */
#define DECLARE_UNIFIED_SUB_PAGE_ARRAY(__memoryBlockSize) \
    sePage##__memoryBlockSize##KiB m_##__memoryBlockSize##KiBBlocks[ GetRequiredLengthToFillUnion(k_BlockSize, __memoryBlockSize) ]

typedef struct seUnifiedPage1KiB
{
    const static size_t k_BlockSize = 1;
    union
    {
        sePage1KiB m_1KiBBlock;
    };
} seUnifiedPage1KiB;

namespace savanna
{
    using UnifiedPage1KiB = seUnifiedPage1KiB;
}

// /**
//  * @brief TODO @David.Mohrhardt Document
//  *
//  */
DECLARE_UNIFIED_PAGE_STRUCT(
    2,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
DECLARE_UNIFIED_PAGE_STRUCT(
    4,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(2);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
DECLARE_UNIFIED_PAGE_STRUCT(
    8,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(4);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(2);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
DECLARE_UNIFIED_PAGE_STRUCT(
    16,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(8);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(4);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(2);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
DECLARE_UNIFIED_PAGE_STRUCT(
    32,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(16);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(8);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(4);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(2);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
DECLARE_UNIFIED_PAGE_STRUCT(
    64,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(32);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(16);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(8);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(4);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(2);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
DECLARE_UNIFIED_PAGE_STRUCT(
    128,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(64);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(32);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(16);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(8);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(4);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(2);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
DECLARE_UNIFIED_PAGE_STRUCT(
    256,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(128);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(64);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(32);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(16);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(8);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(4);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(2);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

/**
 * @brief TODO @David.Mohrhardt Document
 *
 */
DECLARE_UNIFIED_PAGE_STRUCT(
    512,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(256);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(128);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(64);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(32);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(16);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(8);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(4);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(2);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

/**
 * @brief TODO @David.Mohrhardt Document
 */
DECLARE_UNIFIED_PAGE_STRUCT(
    1024,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(512);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(256);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(128);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(64);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(32);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(16);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(8);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(4);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(2);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

/**
 * @brief TODO @David.Mohrhardt Document
 */
DECLARE_UNIFIED_PAGE_STRUCT(
    2048,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1024);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(512);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(256);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(128);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(64);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(32);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(16);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(8);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(4);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(2);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

/**
 * @brief TODO @David.Mohrhardt Document
 */
DECLARE_UNIFIED_PAGE_STRUCT(
    4096,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(2048);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1024);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(512);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(256);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(128);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(64);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(32);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(16);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(8);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(4);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(2);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

typedef void* (*pfn_sePageAlloc)(size_t count);

pfn_sePageAlloc GetPageAllocForSize(size_t size);

#undef DECLARE_UNIFIED_SUB_PAGE_ARRAY
#undef DECLARE_UNIFIED_PAGE_STRUCT
#undef TYPEDEF_PAGE
#undef DECLARE_SAVANNA_KIB_PAGE
#undef USING_SAVANNA_TYPE_CPP
