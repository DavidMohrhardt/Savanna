/**
 * @file AllocatorType.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-14
 *
 */

#pragma once

#include "Utilities/SavannaCoding.h"

enum SavannaAllocatorType
{
    k_None,
    k_FreeList,
    k_Linear,
    k_Stack,
    k_AllocatorTypeCount,
    k_Ensure32Bit = 0xFFFFFFFF
};
DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(SavannaAllocatorType, AllocatorType);
