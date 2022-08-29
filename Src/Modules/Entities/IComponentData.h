/**
 * @file Component.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "ComponentRegistry.h"
#include "IComponent.h"

namespace Savanna::Entities
{
    template<typename T>
    struct IComponentData : public IComponent
    {
        // static_assert(!(std::is_base_of<IComponent>::value), "Template parameter cannot be another IComponent type.");
    public:
        IComponentData() = default;
        virtual ~IComponentData() = default;

        /**
         * @brief Get the Component Id object.
         *
         * @return SAVANNA_NO_DISCARD const&
         */
        SAVANNA_NO_DISCARD virtual const ComponentKey& GetComponentKey() override final;
        SAVANNA_NO_DISCARD inline static const ComponentKey& GetKey();

        bool operator==(const IComponentData& other) const;
        bool operator!=(const IComponentData& other) const;
    };

    template<typename T>
    inline const ComponentKey& IComponentData<T>::GetComponentKey()
    {
        return GetKey();
    }

    template<typename T>
    inline const ComponentKey& IComponentData<T>::GetKey()
    {
        static ComponentKey id =
            ComponentRegistry::RegisterComponentWithTypeIndex(typeid(IComponentData<T>));
        return id;
    }
}
