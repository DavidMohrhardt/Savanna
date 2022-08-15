/**
 * @file ComponentManager.h
 * @author David Mohrhardt (DavidMohrhardt@github.com)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <SavannaEngine.h>
#include <SavannaCoding.h>

namespace Savanna::ECS
{
    class ComponentManager
    {
        // friend class SavannaScriptingApi;
    private:
        SAVANNA_NO_DISCARD static uint64 RegisterComponent(ComponentData<T> componentData) SAVANNA_NO_EXCEPT;

    }
} // namespace Savanna::ECS