/**
 * @file MathTestUtils.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-08
 *
 */
#pragma once

#include "Core/CoreTestUtils.h"

#define DECLARE_MATH_TEST(__subFixture, __name) \
    DECLARE_CORE_TEST(Math_##__subFixture, __name)

#define DECLARE_MATH_TEST_F(__subFixture, __name) \
    DECLARE_CORE_TEST_F(Math_##__subFixture, __name)
