/**
 * @file SavannaAllocator.h
 * @author David Mohrhardt (DavidMohrhardt@github.com)
 * @brief
 * @version 0.1
 * @date 2022-08-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "Utilities/SavannaCoding.h"

#include "AllocatorKind.h"
#include "FreeListAllocator.h"
// #include "LinearAllocator.h"
// #include "StackAllocator.h"

namespace Savanna
{
    class SavannaAllocator
    {
    private:

    public:

        AllocatorKind m_AllocatorKind;

        union
        {
            FreeListAllocator m_FreeListAllocator;
            LinearAllocator m_LinearAllocator;
        };
    };
} // namespace Savanna
