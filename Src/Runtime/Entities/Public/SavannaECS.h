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
#pragma once

#include "SavannaEntities.h"

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
    class ComponentRegistry;

    class IComponent;

    template<typename T>
    class IComponentData;
} // namespace Savanna::Entities

SAVANNA_EXPORT(void) SE_RegisterComponent(void* pComponent);
