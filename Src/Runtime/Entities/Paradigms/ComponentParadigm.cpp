/**
 * @file ComponentParadigm.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Implementation of the ComponentParadigm class.
 * @version 0.1
 * @date 2022-08-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "ComponentParadigm.h"

#include <JobSystem/JobManager.h>

// C Standard Library Includes
#include <cstring>

namespace Savanna::Entities
{
    Paradigm::Paradigm(const Paradigm& other)
    {
        m_ParadigmMemory = other.m_ParadigmMemory;
        memcpy(m_ParadigmKeyChain, other.m_ParadigmKeyChain, sizeof(m_ParadigmKeyChain) * sizeof(ComponentKey));
    }

    Paradigm::Paradigm(Paradigm&& other)
    {
        m_ParadigmMemory = other.m_ParadigmMemory;
        memcpy(m_ParadigmKeyChain, other.m_ParadigmKeyChain, sizeof(m_ParadigmKeyChain) * sizeof(ComponentKey));
        other.m_ParadigmMemory = nullptr;
        other.m_ParadigmMemorySize = 0;
    }

    Paradigm::Paradigm(const void* pParadigmMemory, const size_t& paradigmMemorySize)
        : m_ParadigmMemory(pParadigmMemory)
        , m_ParadigmMemorySize(paradigmMemorySize)
    {
        SAVANNA_ASSERT(pParadigmMemory != nullptr, "Paradigm memory cannot be nullptr.");
        SAVANNA_ASSERT(paradigmMemorySize > 0, "Paradigm memory size must be greater than 0.");

        memset(m_ParadigmKeyChain, 0, sizeof(ComponentKey) * SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS);
    }

    Paradigm::~Paradigm()
    {
        // TODO @DavidMohrhardt Release memory
    }

    struct ComponentMetaData
    {
    private:
        size_t m_TotalSize;
        size_t m_NumberOfComponents;
        const size_t* m_pComponentSizeInfo;
        const size_t* m_pComponentAlignmentInfo;

    public:
        ComponentMetaData(const size_t& totalSize, const size_t& numberOfComponents, const size_t* pComponentSizeInfo, const size_t* pComponentAlignmentInfo)
            : m_TotalSize(totalSize)
            , m_NumberOfComponents(numberOfComponents)
            , m_pComponentSizeInfo(pComponentSizeInfo)
            , m_pComponentAlignmentInfo(pComponentAlignmentInfo)
        {
        }

        inline size_t GetTotalSize() const
        {
            return m_TotalSize;
        }

        inline size_t GetNumberOfComponents() const
        {
            return m_NumberOfComponents;
        }

        inline const size_t* GetComponentSizeInfo() const
        {
            return m_pComponentSizeInfo;
        }

        inline const size_t* GetComponentAlignmentInfo() const
        {
            return m_pComponentAlignmentInfo;
        }
    };

    void Paradigm::UpdateParadigmLayout()
    {
        using namespace Savanna::Concurrency;
        // Paradigm update jobs need to be defined
        // auto layoutUpdateJobFunc = [](void* pUserData) -> JobResult
        // {
        //     Paradigm* pOwnerParadigm = reinterpret_cast<Paradigm*>(pUserData);
        //     JobManager::Get()->AwaitCompletion(pOwnerParadigm->GetParadigmUpdateJobHandleInternal());
        //     pOwnerParadigm->

        //     // Compute the paradigm layout
        //     size_t numberOfComponents = 0;
        //     size_t totalSize = 0;
        //     size_t componentSizeInfo[SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS];
        //     size_t componentAlignmentInfo[SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS];

        //     for (size_t i = 0; i < SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS; ++i)
        //     {
        //         if (m_ParadigmKeyChain[i].GetRingIndex() != 0)
        //         {
        //             const ComponentKey& componentKey = m_ParadigmKeyChain[i];
        //             const ComponentMetaData& componentMetaData = ComponentRegistry::Get()->GetComponentMetaData(componentKey);
        //             componentSizeInfo[numberOfComponents] = componentMetaData.GetTotalSize();
        //             componentAlignmentInfo[numberOfComponents] = componentMetaData.GetNumberOfComponents();
        //             totalSize += componentMetaData.GetTotalSize();
        //             ++numberOfComponents;
        //         }
        //     }

        //     // Compute the number of paradigm buffers needed to transfer the current paradigm to the new layout
        //     size_t numberOfParadigmBuffers = 0;
        //     size_t paradigmBufferSize = 0;
        //     size_t paradigmBufferAlignment = 0;

        //     for (size_t i = 0; i < numberOfComponents; ++i)
        //     {
        //         paradigmBufferSize += componentSizeInfo[i];
        //         paradigmBufferAlignment = std::max(paradigmBufferAlignment, componentAlignmentInfo[i]);
        //         if (paradigmBufferSize > SAVANNA_ECS_MAX_PARADIGM_BUFFER_SIZE)
        //         {
        //             paradigmBufferSize = 0;
        //             ++numberOfParadigmBuffers;
        //         }
        //     }

        //     return JobResult::Failure;
        // });
        // m_ParadigmUpdateJobHandle = JobManager::Get()->ScheduleJob(updateJob);
    }

    // TODO @DavidMohrhardt Remove this, component paradigms should be defined at construction. Reformatting layouts is unnecessary. as the all paradigms are the same size, instead just copy construct a new paradigm on top of the old one.
    void Paradigm::AddComponentToParadigmInternal(
        const size_t& size,
        const size_t& alignment,
        const ComponentKey& componentKey)
    {
        if (m_EntityCount > 0)
        {
            throw RuntimeErrorException("Cannot add new components to paradigms that are already storing entity paradigms.");
        }

        size_t newSize = m_EntityParadigmSize + size;
        if (newSize > m_ParadigmMemorySize)
        {
            // TODO @DavidMohrhardt return an error code instead of throwing an exception.
            throw RuntimeErrorException("Not enough memory to add new component to paradigm.");
        }

        m_EntityParadigmSize = newSize;
        m_ParadigmKeyChain[componentKey.GetRingIndex()] |= componentKey;

        // TODO @DavidMohrhardt consider making this asynchronous
        // as it's likely to involve a lot of memory movement
        UpdateParadigmLayout();
    }

    void Paradigm::AllocateParadigm(size_t paradigmSize)
    {

    }

    ArraySlice<ComponentKey> Paradigm::GetKeyChain() const {
        return ArraySlice<ComponentKey>(m_ParadigmKeyChain, SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS);
    }
} // namespace Savanna::Entities

