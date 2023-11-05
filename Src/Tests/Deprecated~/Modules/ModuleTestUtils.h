/**
 * @file ModuleTestUtils.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-08
 *
 */
#pragma once

#include "SavannaTestRunner.h"

#define DECLARE_MODULE_TEST(__fixture, __name) TEST(Module_##__fixture, __name)

#define DECLARE_MODULE_TEST_F(__fixture, __name) TEST_F(Module_##__fixture, __name)

// TODO @DavidMohrhardt Replace with ifdefs when debugging is enabled
#define NO_DISCARD_EXPECT_COMPARISON(__expect, __func, __actual) \
    { \
        auto __result = __func; \
        __expect(__result, __actual); \
    }

#define NO_DISCARD_THROW_CONDITIONAL(__exceptionTestCondition, __lambda) \
    { \
        __exceptionTestCondition(__lambda); \
    }
