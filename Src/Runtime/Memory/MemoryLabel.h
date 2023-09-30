/**
 * @file MemoryLabel.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Utilities/SavannaCoding.h"

DEFINE_SAVANNA_ENUM(Savanna, se_MemoryLabel_t, MemoryLabel, uint32_t,
    k_SavannaMemoryLabelGeneral,
    k_SavannaMemoryLabelGfx,
    k_SavannaMemoryLabelEcs,

    k_SavannaMemoryLabelCount,

    // This label is used for memory that is allocated from the OS
    // It's not included in the count above because it's not an engine
    // specific label.
    k_SavannaMemoryLabelHeap,
);
