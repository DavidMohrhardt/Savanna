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

#include "Utilities/SavannaCoding.h"

#undef SAVANNA_ECS_KEY_TEETH_BIT_COUNT
#define SAVANNA_ECS_KEY_TEETH_BIT_COUNT 24

#undef SAVANNA_ECS_KEY_RING_INDEX_BIT_COUNT
#define SAVANNA_ECS_KEY_RING_INDEX_BIT_COUNT 8

#undef SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS
#define SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS 255

#undef SAVANNA_COMPONENT_KEY_MAX_VALUE
#define SAVANNA_COMPONENT_KEY_MAX_VALUE ( 0x1 << (SAVANNA_ECS_KEY_TEETH_BIT_COUNT - 1) )

#undef SE_INVALID_HANDLE
#define SE_INVALID_HANDLE k_InvalidComponentKey

#undef SE_INVALID_ID
#define SE_INVALID_ID k_InvalidComponentKey.m_KeyTeeth

typedef se_uint32 se_ComponentKey_t;
typedef se_uint8 se_ComponentKeyMask_T;

// Mask the most significant 8 bits of the component ID to represent additional sets of components.
// (2^8) * (32 - 8) - 1 possible unique component IDs as 0x0ul is reserved for invalid IDs.
// Additionally, making the mask 16 bits will result in (2^16) * (32 - 16) possible unique component IDs.
// To store the sets in a ComponentParadigm you need to store sizeof(ComponentKey) * (2^n).
// So for n = 8 it'd be 8 bytes * 256 = 2048 bytes which is really bad. If it's 32 bits then it'd be
// 4 bytes * 256 = 1024 bytes which is better but still not ideal. On a cacheline size of 64 bytes,
// that's still 1024 / 64 = 16 cache lines to read.

#if defined(__cplusplus)
namespace Savanna::Entities
{
    class ComponentKey
    {
    private:

        ComponentKeyData m_ComponentKey;

    public:
        ComponentKey() : m_ComponentKey({ 0 }) {}
        ComponentKey(const ComponentKeyData& componentKey) : m_ComponentKey(componentKey) {}
        ComponentKey(const ComponentKey& componentKey) : m_ComponentKey(componentKey.m_ComponentKey) {}
        ComponentKey(const se_ComponentKey_t& componentKey) : m_ComponentKey({ componentKey }) {}

        ~ComponentKey() = default;

    public:
        bool CompareKey(const ComponentKey& componentKey) const;

        ComponentKeyData GetKey() const;
        se_ComponentKey_t GetKeyValue() const;
        se_ComponentKeyMask_T GetRingIndex() const;

        void SetKey(const ComponentKeyData& componentKey);
        void SetKey(const se_ComponentKey_t& componentKey);
        void SetRingIndex(const se_ComponentKeyMask_T& componentKeyRingIndex);
        void SetKeyValue(const se_ComponentKey_t& componentKeyValue);

        void Invalidate();

        bool IsValid() const;

    public:
        operator ComponentKeyData() const { return m_ComponentKey; }
        operator se_ComponentKey_t() const { return m_ComponentKey.m_FullComponentKey; }

        ComponentKey& operator=(const ComponentKey& componentKey)
        {
            m_ComponentKey = componentKey.m_ComponentKey;
            return *this;
        }

        ComponentKey& operator=(const ComponentKeyData& componentKey)
        {
            m_ComponentKey = componentKey;
            return *this;
        }

        ComponentKey& operator=(const se_ComponentKey_t& componentKey)
        {
            m_ComponentKey.m_FullComponentKey = componentKey;
            return *this;
        }

        ComponentKey operator |(const ComponentKey& componentKey)
        {
            return ComponentKey(m_ComponentKey.m_FullComponentKey | componentKey.m_ComponentKey.m_FullComponentKey);
        }

        ComponentKey operator |(const ComponentKeyData& componentKey)
        {
            return ComponentKey(m_ComponentKey.m_FullComponentKey | componentKey.m_FullComponentKey);
        }

        ComponentKey operator |(const se_ComponentKey_t& componentKey)
        {
            return ComponentKey(m_ComponentKey.m_FullComponentKey | componentKey);
        }

        ComponentKey& operator |=(const ComponentKey& componentKey)
        {
            m_ComponentKey.m_FullComponentKey |= componentKey.m_ComponentKey.m_FullComponentKey;
            return *this;
        }

        ComponentKey& operator |=(const ComponentKeyData& componentKey)
        {
            m_ComponentKey.m_FullComponentKey |= componentKey.m_FullComponentKey;
            return *this;
        }

        ComponentKey& operator |=(const se_ComponentKey_t& componentKey)
        {
            m_ComponentKey.m_FullComponentKey |= componentKey;
            return *this;
        }

        ComponentKey operator &(const ComponentKey& componentKey)
        {
            return ComponentKey(m_ComponentKey.m_FullComponentKey & componentKey.m_ComponentKey.m_FullComponentKey);
        }

        ComponentKey operator &(const ComponentKeyData& componentKey)
        {
            return ComponentKey(m_ComponentKey.m_FullComponentKey & componentKey.m_FullComponentKey);
        }

        ComponentKey operator &(const se_ComponentKey_t& componentKey)
        {
            return ComponentKey(m_ComponentKey.m_FullComponentKey & componentKey);
        }

        ComponentKey& operator &=(const ComponentKey& componentKey)
        {
            m_ComponentKey.m_FullComponentKey &= componentKey.m_ComponentKey.m_FullComponentKey;
            return *this;
        }

        ComponentKey& operator &=(const ComponentKeyData& componentKey)
        {
            m_ComponentKey.m_FullComponentKey &= componentKey.m_FullComponentKey;
            return *this;
        }

        ComponentKey& operator &=(const se_ComponentKey_t& componentKey)
        {
            m_ComponentKey.m_FullComponentKey &= componentKey;
            return *this;
        }

        ComponentKey operator ^(const ComponentKey& componentKey)
        {
            return ComponentKey(m_ComponentKey.m_FullComponentKey ^ componentKey.m_ComponentKey.m_FullComponentKey);
        }

        ComponentKey operator ^(const ComponentKeyData& componentKey)
        {
            return ComponentKey(m_ComponentKey.m_FullComponentKey ^ componentKey.m_FullComponentKey);
        }

        ComponentKey operator ^(const se_ComponentKey_t& componentKey)
        {
            return ComponentKey(m_ComponentKey.m_FullComponentKey ^ componentKey);
        }

        ComponentKey& operator ^=(const ComponentKey& componentKey)
        {
            m_ComponentKey.m_FullComponentKey ^= componentKey.m_ComponentKey.m_FullComponentKey;
            return *this;
        }

        ComponentKey& operator ^=(const ComponentKeyData& componentKey)
        {
            m_ComponentKey.m_FullComponentKey ^= componentKey.m_FullComponentKey;
            return *this;
        }

        ComponentKey& operator ^=(const se_ComponentKey_t& componentKey)
        {
            m_ComponentKey.m_FullComponentKey ^= componentKey;
            return *this;
        }

        auto operator<=>(const ComponentKey& other) const
        {
            return m_ComponentKey.m_FullComponentKey <=> other.m_ComponentKey.m_FullComponentKey;
        };

        auto operator<=>(const ComponentKeyData& other) const
        {
            return *this <=> ComponentKey(other);
        }

        auto operator<=>(const se_ComponentKey_t& other) const
        {
            return *this <=> ComponentKey(other);
        }
    };
} // namespace Savanna

#endif

#endif
