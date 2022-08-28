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

#undef SE_INVALID_ID
#define SE_INVALID_HANDLE k_InvalidComponentId

#undef SE_INVALID_ID
#define SE_INVALID_ID k_InvalidComponentId.m_ComponentId

const __se_uint64 k_ComponentIdMask = 0x00FFFFFFFFFFFFFFull;
const __se_uint64 k_ComponentIdSetMask = ~k_ComponentIdMask;

const __se_int32 k_ComponentIdKeyBitCount = 56;

/**
 * @brief Defines a unique identifier for a component type. Components of like types are assigned the same
 * identifier.
 */
typedef union SEComponentId
{
    /**
     * @brief The full component identifier.
     */
    __se_uint64 m_ComponentId;

    struct
    {

        /**
         * @brief The key portion of the component id. Used to identify a component type.
         */
        __se_uint64 m_ComponentKey : 56;

        // Mask the most significant 8 bits of the component ID to represent additional sets of components.
        // (2^8) * (64 - 8) = 16392 possible unique component IDs. Real value is 16391 as 0x0ul is reserved for invalid IDs.
        // Additionally, making the mask 16 bits will result in (2^16) * (64 - 16) = 3145728 possible unique component IDs.
        // 64 bits may be overkill. 32 bits would allow for only (2^8) * (32 - 8) = 6144 combinations.
        /**
         * @brief Indicates which component registration set this component ID belongs to.
         */
        __se_uint64 m_Set : 8;
    };
} SEComponentId;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Entities, SEComponentId, ComponentId);

static_assert(sizeof(SEComponentId) == sizeof(__se_uint64), "SEComponentId is not 64 bits");

static const SEComponentId k_InvalidComponentId = { 0x0ull };

inline void se_GetComponentIdSet(const SEComponentId& id, __se_uint64& set)
{
    set = id.m_Set;
}

inline void se_GetBaseId(const SEComponentId& id, __se_uint64& baseId)
{
    baseId = id.m_ComponentKey;
}

inline bool se_IsValidComponentId(const SEComponentId& componentId)
{
    return componentId.m_ComponentId != k_InvalidComponentId.m_ComponentId;
}

inline bool se_CompareKeys(const SEComponentId& entityKey, const SEComponentId& systemLock)
{
    // This is where we should use SIMD to speed up the comparison.
    const __se_uint8 underflowedUint8Max = 0x0 - 1;
    for (int i = 0; i < underflowedUint8Max; ++i)
    {
        if (entityKey.m_Set != systemLock.m_Set)
        {
            return false;
        }
    }

    return true;
}
