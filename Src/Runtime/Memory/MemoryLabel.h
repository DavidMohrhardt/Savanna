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
    SE_MEMORY_LABEL_GENERAL,
    SE_MEMORY_LABEL_GRAPHICS,
    SE_MEMORY_LABEL_ENTITIES,
    // SE_MEMORY_LABEL_CONCURRENCY,

    SE_MEMORY_LABEL_COUNT,

    // The heap label is used for allocations that are explicitly to be allocated from the heap.
    SE_MEMORY_LABEL_HEAP,
);
