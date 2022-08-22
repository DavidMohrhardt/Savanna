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
#define SAVANNA_INVALID_COMPONENT_ID 0x0

const uint8 k_ComponentIdMask = 0xFF;

/**
 * @brief Defines a unique identifier for a component type. Components of like types are assigned the same
 * identifier.
 */
typedef union se_ComponentId alignas(8)
{
    uint8 m_SetMask : 0;
    uint64 m_ComponentId : 0;
} se_ComponentId;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(ECS, se_ComponentId, ComponentId);

bool se_IsValidComponentId(const ComponentId& const componentId)
{
    return (componentId.m_ComponentId & ~k_ComponentIdMask) != SAVANNA_INVALID_COMPONENT_ID;
}
