/**
 * @file EntitiesManager.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-11-05
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include "Public/ISavannaEntities.h"

#include <Types/Manager/GlobalManager.h>

#include <Types/Containers/Arrays/dynamic_array.h>

#include <atomic>

namespace Savanna::Entities
{
    class EntitiesManager final : public GlobalManager<EntitiesManager>
    {
    private:
        DEFINE_GLOBAL_MANAGER_FRIENDS_FOR(EntitiesManager);

        EntitiesManager();
        ~EntitiesManager();

    protected:
        virtual bool InitializeInternal() final;
        virtual void StartInternal() final;
        virtual void StopInternal() final;
        virtual void ShutdownInternal() final;
    };
} // namespace Savanna::Entities
