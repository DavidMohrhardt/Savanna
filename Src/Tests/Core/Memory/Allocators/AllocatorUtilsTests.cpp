/**
 * @file AllocatorUtilsTests.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <gtest/gtest.h>
#include <Memory/Allocators/AllocatorUtils.h>
#include "AllocatorTestUtils.h"

#define DECLARE_ALLOCATOR_UTILS_TEST(__name) \
    DECLARE_ALLOCATOR_TEST(UtilsTest, __name)

namespace Savanna::Core::Tests
{
    DECLARE_ALLOCATOR_UTILS_TEST(GetForwardAlignmentProducesTheCorrectResult)
    {
        constexpr size_t alignment = 8;
        constexpr uintptr ptr = 2;
        size_t forwardAlignment = GetForwardAlignment(reinterpret_cast<void*>(ptr), alignment);
        EXPECT_EQ(alignment - (ptr % alignment), forwardAlignment);
        EXPECT_EQ(forwardAlignment, 6);
    }

    DECLARE_ALLOCATOR_UTILS_TEST(GetForwardAlignmentProducesTheCorrectResultAlignedPtr)
    {
        constexpr size_t alignment = 8;
        constexpr uintptr ptr = 8;
        size_t forwardAlignment = GetForwardAlignment(reinterpret_cast<void*>(ptr), alignment);
        EXPECT_EQ(forwardAlignment, 0);
    }

    DECLARE_ALLOCATOR_UTILS_TEST(GetForwardAlignedPtrProducesTheCorrectResult)
    {
        constexpr size_t alignment = 8;
        constexpr uintptr ptr = 2;
        uintptr forwardAlignedPtr = reinterpret_cast<uintptr>(GetForwardAlignedPtr<void, void>(reinterpret_cast<void*>(ptr), alignment));
        EXPECT_EQ(forwardAlignedPtr, 8);
    }
} // namespace Savanna::Testing::Core::Memory::Allocators
