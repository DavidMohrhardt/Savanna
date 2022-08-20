/**
 * @file AllocatorTestUtils.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-08
 *
 */
#pragma once

#include "Core/Memory/MemoryTestUtils.h"

#include <Memory/Allocators/AllocatorType.h>
#include <Memory/Allocators/AllocatorUtils.h>

#define DECLARE_ALLOCATOR_TEST(__subFixture, __name) \
    DECLARE_MEMORY_TEST(Allocator_##__subFixture, __name)

#define DECLARE_ALLOCATOR_TEST_F(__subFixture, __name) \
    DECLARE_MEMORY_TEST_F(Allocator_##__subFixture, __name)
