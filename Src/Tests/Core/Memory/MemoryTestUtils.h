/**
 * @file MemoryTestUtils.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "Core/CoreTestUtils.h"

#define DECLARE_MEMORY_TEST(__subFixture, __name) \
    DECLARE_CORE_TEST(Memory_##__subFixture, __name)

#define DECLARE_MEMORY_TEST_F(__subFixture, __name) \
    DECLARE_CORE_TEST_F(Memory_##__subFixture, __name)
