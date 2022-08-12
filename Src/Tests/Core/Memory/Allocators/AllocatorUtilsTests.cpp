/**
 * @file AllocatorUtilsTests.cpp
 * @author David Mohrhardt (david.mohrhardt@gmail.com)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <gtest/gtest.h>
#include <Memory/Allocators/AllocatorUtils.h>
#include <Types/Pointers/UniversalPointer.h>
#include "AllocatorTestUtils.h"

#define DECLARE_ALLOCATOR_UTILS_TEST(__name) \
    DECLARE_ALLOCATOR_TEST(UtilsTest, __name)

namespace Savanna::Core::Tests
{
    DECLARE_ALLOCATOR_UTILS_TEST(GetForwardAlignmentProducesTheCorrectResult)
    {
        constexpr size_t alignment = 8;
        s_universalptr ptr = {.m_UIntPtr = 2};
        size_t forwardAlignment = GetForwardAlignment(ptr.m_Ptr, alignment);
        EXPECT_EQ(alignment - (ptr.m_UIntPtr % alignment), forwardAlignment);
        EXPECT_EQ(forwardAlignment, 6);
    }

    DECLARE_ALLOCATOR_UTILS_TEST(GetForwardAlignmentProducesTheCorrectResultAlignedPtr)
    {
        constexpr size_t alignment = 8;
        s_universalptr ptr = {.m_UIntPtr = 8};
        size_t forwardAlignment = GetForwardAlignment(ptr.m_Ptr, alignment);
        EXPECT_EQ(forwardAlignment, 0);
    }

    DECLARE_ALLOCATOR_UTILS_TEST(GetForwardAlignedPtrProducesTheCorrectResult)
    {
        constexpr size_t alignment = 8;
        s_universalptr ptr = {.m_UIntPtr = 2};
        s_universalptr alignedPtr = {.m_Ptr = GetForwardAlignedPtr<void>(ptr.m_Ptr, alignment)};
        EXPECT_EQ(alignedPtr.m_UIntPtr, 8);
    }
} // namespace Savanna::Testing::Core::Memory::Allocators
