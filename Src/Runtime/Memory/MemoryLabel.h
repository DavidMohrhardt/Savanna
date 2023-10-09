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
    // This label is used for memory that is allocated from the OS
    // It's not included in the count above because it's not an engine
    // specific label.
    k_SavannaMemoryLabelHeap,
    k_SavannaMemoryLabelGeneral,
    k_SavannaMemoryLabelGfx,
    k_SavannaMemoryLabelEcs,

    k_SavannaMemoryLabelCount,

    // Mask the none label using the count to reuse the count value
    k_SavannaMemoryLabelNone = k_SavannaMemoryLabelCount,
);

DEFINE_SAVANNA_ENUM(Savanna, se_MemoryArenaId_t, ArenaId, uint32_t,
    k_SavannaMemoryArenaIdGeneral,
    k_SavannaMemoryArenaIdGfx,
    k_SavannaMemoryArenaIdECS,
    k_SavannaMemoryArenaIdCount,
    k_SavannaMemoryArenaIdInvalid = k_SavannaMemoryArenaIdCount,
);

inline se_MemoryArenaId_t SavannaMemoryGetArenaIdFromLabel(uint32_t label)
{
    switch (label)
    {
    case k_SavannaMemoryLabelGeneral:
        return k_SavannaMemoryArenaIdGeneral;
    case k_SavannaMemoryLabelGfx:
        return k_SavannaMemoryArenaIdGfx;
    case k_SavannaMemoryLabelEcs:
        return k_SavannaMemoryArenaIdECS;
    default:
        return k_SavannaMemoryArenaIdInvalid;
    }
}

inline se_MemoryLabel_t SavannaMemoryGetLabelFromArenaId(uint32_t id)
{
    switch (id)
    {
    case k_SavannaMemoryArenaIdGeneral:
        return k_SavannaMemoryLabelGeneral;
    case k_SavannaMemoryArenaIdGfx:
        return k_SavannaMemoryLabelGfx;
    case k_SavannaMemoryArenaIdECS:
        return k_SavannaMemoryLabelEcs;
    default:
        return k_SavannaMemoryLabelNone;
    }
}

inline const char* SavannaEngineGetMemoryArenaName(se_uint32 arenaId)
{
    switch (arenaId)
    {
    case k_SavannaMemoryArenaIdGeneral:
        return "General";
    case k_SavannaMemoryArenaIdGfx:
        return "Gfx";
    case k_SavannaMemoryArenaIdECS:
        return "Ecs";
    default:
        return "Unknown";
    }
}

inline const char* SavannaEngineGetMemoryLabelName(se_uint32 label)
{
    switch (label)
    {
    case k_SavannaMemoryLabelGeneral:
        return "General";
    case k_SavannaMemoryLabelGfx:
        return "Gfx";
    case k_SavannaMemoryLabelEcs:
        return "Ecs";
    case k_SavannaMemoryLabelHeap:
        return "Heap";
    default:
        return "Unknown";
    }
}
