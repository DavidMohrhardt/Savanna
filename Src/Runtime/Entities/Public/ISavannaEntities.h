/**
 * @file ISavannaEntities.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_ENTITIES_H
#define I_SAVANNA_ENTITIES_H

#include <Public/ISavannaEngine.h>

/**
 * @brief The entity handle type.
 *
 */
typedef struct se_Entity_t
{
    se_uint32 m_Index;
    se_uint32 m_Version;
} se_Entity_t;

#undef SAVANNA_ECS_KEY_TEETH_BIT_COUNT
#define SAVANNA_ECS_KEY_TEETH_BIT_COUNT 24

#undef SAVANNA_ECS_KEY_RING_INDEX_BIT_COUNT
#define SAVANNA_ECS_KEY_RING_INDEX_BIT_COUNT 8

#undef SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS
#define SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS 255

#undef SAVANNA_COMPONENT_KEY_MAX_VALUE
#define SAVANNA_COMPONENT_KEY_MAX_VALUE ( 0x1 << (SAVANNA_ECS_KEY_TEETH_BIT_COUNT - 1) )

#undef SE_INVALID_COMPONENT_KEY
#define SE_INVALID_COMPONENT_KEY ((se_ComponentKey_t)0x0)

#undef SE_INVALID_COMPONENT_KEY_TEETH
#define SE_INVALID_COMPONENT_KEY_TEETH (SE_INVALID_COMPONENT_KEY).m_KeyTeeth

/**
 * @brief Error codes returned by a given entity system.
 *
 */
typedef enum se_EntitySystemErrorCode_t
{
    kSavannaSystemSuccess,
    kSavannaSystemFailure,

    kSavannaSystemMalformedInput,

    kSavannaSystemAlreadyInitialized,

    kSavannaSystemEnsure32Bits = 0xFFFFFFFF
} se_EntitySystemErrorCode_t;

/**
 * @brief The component key union type. Is a 32-bit bitset with a reserved 8 bits for the component registration index
 * and 24 bits for the component key.
 *
 * @details Mask the most significant 8 bits of the component ID to represent additional sets of components.
 * (2^8) * (32 - 8) - 1 possible unique component IDs as 0x0ul is reserved for invalid IDs.
 * Additionally, making the mask 16 bits will result in (2^16) * (32 - 16) - 1 possible unique component IDs.
 * To store the sets in a ComponentParadigm you need to store sizeof(ComponentKey) * (2^n).
 * So for n = 8 it'd be 8 bytes * 256 = 2048 bytes which is really bad. If it's 32 bits then it'd be
 * 4 bytes * 256 = 1024 bytes which is better but still not ideal. On a cacheline size of 64 bytes,
 * that's still 1024 / 64 = 16 cache lines to read.
 *
 */
typedef union se_ComponentKey_t
{
    /**
     * @brief The full component key.
     */
    se_uint32 m_FullComponentKey;

    struct
    {
        /**
         * @brief The key portion of the component id. Used to identify a component type.
         */
        se_uint32 m_KeyTeeth : SAVANNA_ECS_KEY_TEETH_BIT_COUNT;

        /**
         * @brief Indicates which component registration set this component ID belongs to.
         */
        se_uint32 m_RingIndex : SAVANNA_ECS_KEY_RING_INDEX_BIT_COUNT;
    };
} se_ComponentKey_t;


/**
 * @brief The entity system interface.
 *
 */
typedef struct se_EntitySystemInterface_t
{
    /**
     * @brief The system lock used to determine if the
     * system can operate on a given entity.
     *
     */
    se_ComponentKey_t m_SystemLock;

    /**
     * @brief Runs the system.
     *
     * @param pComponentBatch The component batch the system should operator on.
     * @return se_EntitySystemErrorCode_t The error code.
     */
    se_EntitySystemErrorCode_t (*se_pfnRunSystem)(void* pSystemBatch);
} se_EntitySystemInterface_t;

typedef struct se_EntitySystemCreateInfo_t
{
    /**
     * @brief The system interface.
     *
     */
    se_EntitySystemInterface_t* m_pSystemInterface;
} se_EntitySystemCreateInfo_t;

// API

SAVANNA_EXPORT(bool) SavannaEntitiesIsValidComponentKey(const se_ComponentKey_t& componentId);

SAVANNA_EXPORT(bool) SavannaEntitiesCompareKeys(const se_ComponentKey_t& entityKey, const se_ComponentKey_t& systemLock);

#endif // I_SAVANNA_ENTITIES_H
