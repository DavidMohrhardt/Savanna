/**
 * @file ECSBindings.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-21
 */
#pragma once

#include "Entities.h"

#include <Utilities/SavannaCoding.h>

// While realistically extern "C" does not respect namespace, it's just easier if C++ functions
// need to be defined here as well as the public API.

/**
 * @brief Registers a script defined component type to the component registry.
 */
SAVANNA_EXPORT(const ComponentKey) SavannaRegisterExternalComponent(IComponent* component);

/**
 * @brief Registers a script defined component type to the component registry.
 */
SAVANNA_EXPORT(const ComponentKey) SavannaRegisterExternalComponent(const ComponentKey& componentId);
