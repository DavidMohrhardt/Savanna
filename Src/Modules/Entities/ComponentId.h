/**
 * @file ComponentId.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <Utilities/SavannaCoding.h>

#undef SAVANNA_INVALID_COMPONENT_ID
#define SAVANNA_INVALID_COMPONENT_ID k_InvalidComponentId;

const __se_uint8 k_ComponentIdMask = 0xFF;

/**
 * @brief Defines a unique identifier for a component type. Components of like types are assigned the same
 * identifier.
 */
typedef union se_ComponentId
{
    __se_uint8 m_SetMask;
    __se_uint64 m_ComponentId;
} se_ComponentId;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(ECS, se_ComponentId, ComponentId);

static const se_ComponentId k_InvalidComponentId = { .m_ComponentId = 0 };

inline bool se_IsValidComponentId(const se_ComponentId& componentId)
{
    return (componentId.m_ComponentId & ~k_ComponentIdMask) != k_InvalidComponentId.m_ComponentId;
}
