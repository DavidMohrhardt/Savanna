/**
 * @file SavannaECS.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Provides public API for the Savanna ECS.
 * @version 0.1
 * @date 2022-08-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef I_SAVANNA_ECS_HPP
#define I_SAVANNA_ECS_HPP

#include "ISavannaEntities.h"

#ifndef SAVANNA_DECLARE_ECS_COMPONENT_STRUCT

/**
 * @brief Helper macro for defining a component type. This macro produces a struct with the name of the
 * component type that inherits from the IComponentData interface.
 */
#define SAVANNA_DECLARE_ECS_COMPONENT_STRUCT(__componentName) \
    struct __componentName : public IComponentData<__componentName>

#endif

namespace Savanna::Entities
{
    /**
     * @brief Represents an invalid component key.
     */
    constexpr se_ComponentKey_t k_InvalidComponentKey = { 0x0 };

    /**
     * @brief Represents the maximum value a component key can have.
     */
    constexpr se_ComponentKey_t k_MaxKeyValue = (se_ComponentKey_t)(0x1 << (SAVANNA_ECS_KEY_TEETH_BIT_COUNT - 1));

    /**
     * @brief Represents the maximum value the ring index can have.
     */
    constexpr se_ComponentKey_t k_ComponentKeyDataRingIndexMask = { 0xFF000000u };

    /**
     * @brief Represents the maximum value the teeth can have.
     */
    constexpr se_ComponentKey_t k_ComponentKeyDataTeethMask = { 0x00FFFFFFu };

    inline bool operator==(const se_ComponentKey_t& lhs, const se_ComponentKey_t& rhs)
    {
        return lhs.m_FullComponentKey == rhs.m_FullComponentKey;
    }

    inline bool operator!=(const se_ComponentKey_t& lhs, const se_ComponentKey_t& rhs)
    {
        return !(lhs == rhs);
    }

    inline se_ComponentKey_t& operator|=(se_ComponentKey_t& lhs, const se_ComponentKey_t& rhs)
    {
        lhs.m_FullComponentKey |= rhs.m_FullComponentKey;
        return lhs;
    }

    inline se_ComponentKey_t& operator&=(se_ComponentKey_t& lhs, const se_ComponentKey_t& rhs)
    {
        lhs.m_FullComponentKey &= rhs.m_FullComponentKey;
        return lhs;
    }

    class ComponentRegistry;

    struct IComponent
    {
    public:
        IComponent() {}
        virtual ~IComponent() = default;
        virtual void* GetRawComponentData(size_t& outSizeOfComponent) = 0;
    };

    template<typename T>
    struct IComponentData final : public IComponent
    {
    private:
        T m_ComponentData;

    public:
        IComponentData() = default;
        virtual ~IComponentData() = default;

        inline static const se_ComponentKey_t& GetKey()
        {
            static auto key = [=]() -> se_ComponentKey_t
            {
                return ComponentRegistry::RegisterComponent();
            }();
            return key;
        }

        // bool operator==(const IComponentData& other) const;
        // bool operator!=(const IComponentData& other) const;
    };


} // namespace Savanna::Entities

SAVANNA_EXPORT(void) SavannaEntitiesRegisterComponent(void* pComponent);

#endif // I_SAVANNA_ECS_HPP
