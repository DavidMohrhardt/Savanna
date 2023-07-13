/**
 * @file JobRunner.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-02-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"

#include "ISavannaJobs.h"

namespace Savanna::Concurrency
{
    class JobRunner
    {
    private:
        IJob* m_pJob;
        JobHandle m_Dependency;

    public:
        JobRunner(IJob* pJob, JobHandle dependency = k_InvalidJobHandle);
        ~JobRunner();

    public:
        void Run();
    };
} // namespace Savanna::Concurrency
