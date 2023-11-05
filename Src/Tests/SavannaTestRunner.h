/**
 * @file SavannaTestRunner.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-08
 *
 */
#pragma once

// General includes
#include <gtest/gtest.h>

// Include the console for output if needed
#include <Utilities/Console.h>

#define SAVANNA_DECLARE_TEST(__fixture, __name) TEST(Savanna_##__fixture, __name)

#define SAVANNA_DECLARE_TEST_F(__fixture, __name) TEST_F(Savanna_##__fixture, __name)

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
