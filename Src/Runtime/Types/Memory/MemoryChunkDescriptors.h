/**
 * @file MemoryChunkDescriptors.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-02-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Utilities/SavannaCoding.h"

#define ENABLE_POISON 0
#define ENABLE_POISON_HEADER ENABLE_POISON && true
#define ENABLE_POISON_FOOTER ENABLE_POISON && true

#if ENABLE_POISON_HEADER
constexpr uintptr_t k_PoisonBytes = 0x4445414442454546;
constexpr uintptr_t k_PoisonHeader = 0x4445414444454144;
constexpr uintptr_t k_PoisonFooter = 0x4245454642454546;
#endif // ENABLE_POISON_HEADER

#if ENABLE_POISON_HEADER
#   define POISON_CHK_HEADER SAVANNA_ASSERT(m_PoisonHeader == k_PoisonHeader, "Memory chunk header poison overwritten");
#else
#   define POISON_CHK_HEADER
#endif // ENABLE_POISON_HEADER
#if ENABLE_POISON_FOOTER
#   define POISON_CHK_FOOTER SAVANNA_ASSERT(m_PoisonFooter == k_PoisonFooter, "Memory chunk footer poison overwritten");
#else
#   define POISON_CHK_FOOTER
#endif // ENABLE_POISON_FOOTER

#define CHECK_POISON() \
    POISON_CHK_HEADER \
    POISON_CHK_FOOTER


namespace Savanna
{
    struct alignas(8) MemoryChunkDescriptor
    {
#if ENABLE_POISON_HEADER
        uintptr_t m_PoisonHeader = k_PoisonHeader;
#endif // ENABLE_POISON_HEADER
        MemoryChunkDescriptor* m_Next = nullptr;
        size_t m_Size = 0;
#if ENABLE_POISON_FOOTER
        uintptr_t m_PoisonFooter = k_PoisonFooter;
#endif // ENABLE_POISON_FOOTER

        MemoryChunkDescriptor() = default;

        MemoryChunkDescriptor(size_t size)
            : m_Size { size }
        {}

        MemoryChunkDescriptor(size_t size, MemoryChunkDescriptor* next)
            : m_Next { next }
            , m_Size { size }
        {}

        MemoryChunkDescriptor(const MemoryChunkDescriptor&) = default;
        MemoryChunkDescriptor(MemoryChunkDescriptor&&) = default;
        MemoryChunkDescriptor& operator=(const MemoryChunkDescriptor&) = default;
        MemoryChunkDescriptor& operator=(MemoryChunkDescriptor&&) = default;
        ~MemoryChunkDescriptor() = default;

        bool IsAdjacent(const MemoryChunkDescriptor* pOther) const
        {
            CHECK_POISON();
            return (reinterpret_cast<const byte*>(this) + sizeof(MemoryChunkDescriptor) + m_Size) == reinterpret_cast<const byte*>(pOther);
        }

        bool IsGreaterOrEqual(const void* pOther) const
        {
            CHECK_POISON();
            return reinterpret_cast<const uintptr>(this) >= reinterpret_cast<const uintptr>(pOther);
        }

        bool IsPerfectFit(size_t size) const
        {
            CHECK_POISON();
            return m_Size == size;
        }
    };

    struct alignas(8) AllocationDescriptor
    {
#if ENABLE_POISON_HEADER
        uintptr_t m_PoisonHeader = k_PoisonBytes;
#endif // ENABLE_POISON_HEADER
        size_t m_Offset = 0;
        size_t m_Size = 0;
#if ENABLE_POISON_FOOTER
        uintptr_t m_PoisonFooter = k_PoisonBytes;
#endif // ENABLE_POISON_FOOTER

        AllocationDescriptor() = default;
        AllocationDescriptor(size_t offset, size_t size) : m_Offset { offset }, m_Size { size } {}
        ~AllocationDescriptor() = default;
    };

    static_assert(sizeof(MemoryChunkDescriptor) == sizeof(AllocationDescriptor), "MemoryChunkDescriptor and AllocatedChunkDescriptor must be the same size");
}
