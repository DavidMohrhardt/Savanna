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
 * @brief
 */
#define DECLARE_SAVANNA_KIB_PAGE(__kibLength) \
    typedef struct alignas( L1CacheLineLength() ) se_Page##__kibLength##KiB_t \
    { \
        union \
        { \
            alignas( L1CacheLineLength() ) se_byte m_##__kibLength##Block[KibiBytesToBytes(__kibLength)]; \
            alignas( L1CacheLineLength() ) se_L1CacheLine m_##__kibLength##CacheLines[ GetL1CacheLineCount( KibiBytesToBytes(__kibLength) ) ]; \
        }; \
    } se_Page##__kibLength##KiB_t; \
    namespace Savanna { using Page##__kibLength##KiB = se_Page##__kibLength##KiB_t; }

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
 * @brief
 */
#define DECLARE_UNIFIED_PAGE_STRUCT(__memoryBlockSize, __unionMembers) \
    typedef struct se_UnifiedPage##__memoryBlockSize##KiB_t \
    { \
        const static size_t k_BlockSize = __memoryBlockSize; \
        union \
        { \
            se_Page##__memoryBlockSize##KiB_t m_##__memoryBlockSize##KiBBlock; \
            __unionMembers \
        }; \
    } se_UnifiedPage##__memoryBlockSize##KiB_t; \
    namespace Savanna { using UnifiedPage##__memoryBlockSize##KiB = se_UnifiedPage##__memoryBlockSize##KiB_t; }

/**
 * @brief Creates an array of se_UnifiedPages that will fit to the size of the union of the given integer size in KiB.
 */
#define DECLARE_UNIFIED_SUB_PAGE_ARRAY(__memoryBlockSize) \
    se_Page##__memoryBlockSize##KiB_t m_##__memoryBlockSize##KiBBlocks[ GetRequiredLengthToFillUnion(k_BlockSize, __memoryBlockSize) ]

typedef struct se_UnifiedPage1KiB_t
{
    const static size_t k_BlockSize = 1;
    union
    {
        se_Page1KiB_t m_1KiBBlock;
    };
} se_UnifiedPage1KiB_t;

namespace Savanna
{
    using UnifiedPage1KiB = se_UnifiedPage1KiB_t;
}

// /**
//  * @brief
//  *
//  */
DECLARE_UNIFIED_PAGE_STRUCT(
    2,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

/**
 * @brief
 *
 */
DECLARE_UNIFIED_PAGE_STRUCT(
    4,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(2);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

/**
 * @brief
 *
 */
DECLARE_UNIFIED_PAGE_STRUCT(
    8,
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(4);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(2);
    DECLARE_UNIFIED_SUB_PAGE_ARRAY(1);
);

/**
 * @brief
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
 * @brief
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
 * @brief
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
 * @brief
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
 * @brief
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
 * @brief
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
 * @brief
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
 * @brief
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
 * @brief
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

typedef void* (*se_PageAllocFunc_t)(size_t count);
namespace Savanna
{
    using PageAllocFunc = se_PageAllocFunc_t;
} // namespace Savanna

se_PageAllocFunc_t GetPageAllocFuncForSize(size_t size);

#undef DECLARE_UNIFIED_SUB_PAGE_ARRAY
#undef DECLARE_UNIFIED_PAGE_STRUCT
#undef TYPEDEF_PAGE
#undef DECLARE_SAVANNA_KIB_PAGE
#undef USING_SAVANNA_TYPE_CPP
