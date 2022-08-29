/**
 * @file ComponentKey.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __SAVANNA_COMPONENT_KEY_H__
#define __SAVANNA_COMPONENT_KEY_H__

#include <Utilities/SavannaCoding.h>

#undef SAVANNA_ECS_KEY_BIT_COUNT
#define SAVANNA_ECS_KEY_BIT_COUNT 24

#undef SAVANNA_ECS_KEY_SET_BIT_COUNT
#define SAVANNA_ECS_KEY_SET_BIT_COUNT 8

#undef SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS
#define SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS (1 << SAVANNA_ECS_KEY_SET_BIT_COUNT)

#undef SAVANNA_COMPONENT_KEY_MAX_VALUE
#define SAVANNA_COMPONENT_KEY_MAX_VALUE (1 << SAVANNA_ECS_KEY_BIT_COUNT)

#undef SE_INVALID_HANDLE
#define SE_INVALID_HANDLE k_InvalidComponentKey

#undef SE_INVALID_ID
#define SE_INVALID_ID k_InvalidComponentKey.m_Key

typedef __se_uint32 se_ComponentKey_t;
typedef __se_uint8 se_ComponentKeyMask_T;

// Mask the most significant 8 bits of the component ID to represent additional sets of components.
// (2^8) * (64 - 8) - 1 possible unique component IDs as 0x0ul is reserved for invalid IDs.
// Additionally, making the mask 16 bits will result in (2^16) * (64 - 16) possible unique component IDs.
// To store the sets in a ComponentParadigm you need to store sizeof(ComponentKey) * (2^n).
// So for n = 8 it'd be 8 bytes * 256 = 2048 bytes which is really bad. If it's 32 bits then it'd be
// 4 bytes * 256 = 1024 bytes which is better but still not ideal. On a cacheline size of 64 bytes,
// that's still 1024 / 64 = 16 cache lines to read

/**
 * @brief Defines a unique key/identifier for a component type. Components of like types are assigned the same
 * key.
 */
typedef union SavannaComponentKey
{
    /**
     * @brief The full component key.
     */
    se_ComponentKey_t m_FullComponentKey;

    /**
     * @brief The key portion of the component id. Used to identify a component type.
     */
    se_ComponentKey_t m_Key : SAVANNA_ECS_KEY_BIT_COUNT;

    /**
     * @brief Indicates which component registration set this component ID belongs to.
     */
    se_ComponentKeyMask_T m_Set : SAVANNA_ECS_KEY_SET_BIT_COUNT;

} SavannaComponentKey;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Entities, SavannaComponentKey, ComponentKey);

static_assert(sizeof(SavannaComponentKey) == sizeof(se_ComponentKey_t), "SavannaComponentKey is not 32 bits");

/**
 * @brief Represents an invalid component key.
 */
const SavannaComponentKey k_InvalidComponentKey = { 0x0 };

const SavannaComponentKey k_SetMask = { 0xFF000000 };

const SavannaComponentKey k_KeyMask = { 0x00FFFFFF };

bool SavannaIsValidComponentKey(const SavannaComponentKey& componentId);

bool SavannaCompareKeys(const SavannaComponentKey& entityKey, const SavannaComponentKey& systemLock);

#endif
