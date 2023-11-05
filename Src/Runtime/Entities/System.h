/**
 * @file System.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "SavannaEntities.h"

#include "SavannaEngine.h"

// #include

namespace Savanna::Entities
{
    class System final
    {
    private:
        const se_EntitySystemInterface_t* m_pInterface { nullptr };
        void* m_pComponentBufferSpace { nullptr };

    public:
        System() = default;
        ~System() = default;

        System(const System&) = delete;
        System& operator=(const System&) = delete;

        System(System&&) = delete;
        System& operator=(System&&) = delete;

        se_EntitySystemErrorCode_t Initialize(const se_EntitySystemInterface_t* pInterface) noexcept;
    };
}
