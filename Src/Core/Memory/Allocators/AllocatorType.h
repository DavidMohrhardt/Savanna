/**
 * @file AllocatorType.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "Utilities/SavannaCoding.h"

enum SavannaAllocatorType
{
    k_FreeList,
    k_Linear,
    k_Stack,
    k_AllocatorTypeCount
}
