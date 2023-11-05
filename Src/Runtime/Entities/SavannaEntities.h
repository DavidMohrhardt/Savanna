/**
 * @file SavannaEntities.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Includes all required header files to create Entities, Components, or Systems
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "Public/ISavannaEntities.hpp"

#include "ComponentParadigm.h"

namespace Savanna::Entities
{
    using Entity = se_Entity_t;
    using ComponentKey = se_ComponentKey_t;
} // namespace Savanna::Entities
