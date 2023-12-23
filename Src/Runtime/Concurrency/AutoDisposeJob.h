/**
 * @file AutoDisposeJob.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-11-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <SavannaEngine.h>
#include <Memory/SavannaMemory.h>
#include "JobSystem.h"

#include <Types/Containers/Arrays/dynamic_array.h>

namespace savanna::Concurrency
{
    class AutoDisposeJobBase : public IJob
    {
    private:
        void Dispose();

        AllocatorKind m_AllocatorKind;

    public:
        AutoDisposeJobBase(AllocatorKind allocatorKind = kSavannaAllocatorKindGeneral);

        AutoDisposeJobBase() = delete;
        virtual ~AutoDisposeJobBase() = default;

        void OnComplete() final;
        void OnError() final;
        void OnCancel() final;
    };

    class AutoDisposePrimitiveJob final : public AutoDisposeJobBase
    {
    private:
        const se_JobDefinition_t m_JobDefinition;
    public:
        AutoDisposePrimitiveJob(const se_JobDefinition_t &jobDefinition, AllocatorKind allocatorKind);

        AutoDisposePrimitiveJob() = delete;

        virtual ~AutoDisposePrimitiveJob() = default;

        JobResult Execute() override;
    };
} // namespace savanna::Concurrency

