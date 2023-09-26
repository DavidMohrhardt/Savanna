/**
 * @file MemoryTestUtils.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-08
 *
 */
#pragma once

#include <Entities/SavannaEntities.h>

#include "Modules/ModuleTestUtils.h"

#define DECLARE_ENTITIES_TEST(__subFixture, __name) \
    DECLARE_MODULE_TEST(Memory_##__subFixture, __name)

#define DECLARE_ENTITIES_TEST_F(__subFixture, __name) \
    DECLARE_MODULE_TEST_F(Memory_##__subFixture, __name)
