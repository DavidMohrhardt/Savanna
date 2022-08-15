/**
 * @file MathHelpersTests.h
 * @author David Mohrhardt (DavidMohrhardt@github.com)
 * @brief Tests for the functions in the Core/Math/MathHelpers.h file
 * @version 0.1
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "MathTestUtils.h"

#include <gtest/gtest.h>
#include <Math/MathHelpers.h>

#define MATH_HELPERS_TEST(__name) \
    DECLARE_MATH_TEST(MathHelpersTest, __name)

// #define MATH_HELPERS_TEST_F(__name) \
//     DECLARE_MATH_TEST_F(MathHelpers, __name)

namespace Savanna::Core::Tests
{
    /**
     * @brief Tests if IsPowerOfTwo will properly return false on non-power-of-two values.
     */
    MATH_HELPERS_TEST(IsPowerOfTwoFailsWhenZero)
    {
        EXPECT_FALSE(IsPowerOfTwo(0));
    }

    MATH_HELPERS_TEST(IsPowerOfTwoFailsWhenNegative)
    {
        EXPECT_FALSE(IsPowerOfTwo(-1));
    }

    MATH_HELPERS_TEST(IsPowerOfTwoFailsWhenNotPowerOfTwo)
    {
        EXPECT_FALSE(IsPowerOfTwo(3));
    }

    MATH_HELPERS_TEST(IsPowerOfTwoWorksForManyPowersOfTwo)
    {
        EXPECT_TRUE(IsPowerOfTwo(1));
        EXPECT_TRUE(IsPowerOfTwo(2));
        EXPECT_TRUE(IsPowerOfTwo(4));
        EXPECT_TRUE(IsPowerOfTwo(8));
        EXPECT_TRUE(IsPowerOfTwo(16));
        EXPECT_TRUE(IsPowerOfTwo(32));
        EXPECT_TRUE(IsPowerOfTwo(64));
        EXPECT_TRUE(IsPowerOfTwo(128));
        EXPECT_TRUE(IsPowerOfTwo(256));
        EXPECT_TRUE(IsPowerOfTwo(512));
        EXPECT_TRUE(IsPowerOfTwo(1024));
        EXPECT_TRUE(IsPowerOfTwo(2048));
        EXPECT_TRUE(IsPowerOfTwo(4096));
        EXPECT_TRUE(IsPowerOfTwo(8192));
        EXPECT_TRUE(IsPowerOfTwo(16384));
        EXPECT_TRUE(IsPowerOfTwo(32768));
        EXPECT_TRUE(IsPowerOfTwo(65536));
        EXPECT_TRUE(IsPowerOfTwo(131072));
        EXPECT_TRUE(IsPowerOfTwo(262144));
        EXPECT_TRUE(IsPowerOfTwo(524288));
        EXPECT_TRUE(IsPowerOfTwo(1048576));
        EXPECT_TRUE(IsPowerOfTwo(2097152));
        EXPECT_TRUE(IsPowerOfTwo(4194304));
        EXPECT_TRUE(IsPowerOfTwo(8388608));
        EXPECT_TRUE(IsPowerOfTwo(16777216));
        EXPECT_TRUE(IsPowerOfTwo(33554432));
        EXPECT_TRUE(IsPowerOfTwo(67108864));
        EXPECT_TRUE(IsPowerOfTwo(134217728));
        EXPECT_TRUE(IsPowerOfTwo(268435456));
        EXPECT_TRUE(IsPowerOfTwo(536870912));
        EXPECT_TRUE(IsPowerOfTwo(1073741824));
    }

    MATH_HELPERS_TEST(ModByPowerOfTwoIsCalculatedCorrectlyModByOne)
    {
        NO_DISCARD_EXPECT_COMPARISON(EXPECT_EQ, ModByPowerOfTwo(1, 1), 0);
        NO_DISCARD_EXPECT_COMPARISON(EXPECT_EQ, ModByPowerOfTwo(100000000, 1), 0);
    }

    MATH_HELPERS_TEST(ModByPowerOfTwoSucceedsSequentiallyModTwo)
    {
        NO_DISCARD_EXPECT_COMPARISON(EXPECT_EQ, ModByPowerOfTwo(1, 2), 1);
        NO_DISCARD_EXPECT_COMPARISON(EXPECT_EQ, ModByPowerOfTwo(2, 2), 0);
        NO_DISCARD_EXPECT_COMPARISON(EXPECT_EQ, ModByPowerOfTwo(3, 2), 1);
        NO_DISCARD_EXPECT_COMPARISON(EXPECT_EQ, ModByPowerOfTwo(4, 2), 0);
    }

    MATH_HELPERS_TEST(ModByPowerOfTwoSucceedsModuloBySixtyFour)
    {
        NO_DISCARD_EXPECT_COMPARISON(EXPECT_EQ, ModByPowerOfTwo(1, 64), 1);
        NO_DISCARD_EXPECT_COMPARISON(EXPECT_EQ, ModByPowerOfTwo(2, 64), 2);
        NO_DISCARD_EXPECT_COMPARISON(EXPECT_EQ, ModByPowerOfTwo(3, 64), 3);
        // ...
        NO_DISCARD_EXPECT_COMPARISON(EXPECT_EQ, ModByPowerOfTwo(63, 64), 63);
        NO_DISCARD_EXPECT_COMPARISON(EXPECT_EQ, ModByPowerOfTwo(64, 64), 0);
        NO_DISCARD_EXPECT_COMPARISON(EXPECT_EQ, ModByPowerOfTwo(65, 64), 1);
        NO_DISCARD_EXPECT_COMPARISON(EXPECT_EQ, ModByPowerOfTwo(66, 64), 2);
        NO_DISCARD_EXPECT_COMPARISON(EXPECT_EQ, ModByPowerOfTwo(67, 64), 3);
    }

    MATH_HELPERS_TEST(ModByPowerOfTwoDiesWhenModulusIsntByPowerOfTwo)
    {
        EXPECT_DEATH_IF_SUPPORTED(
            {
                auto x = ModByPowerOfTwo(1, 3);
            }, ".*");
    }

} // namespace Savanna::Tests::Core::Math
