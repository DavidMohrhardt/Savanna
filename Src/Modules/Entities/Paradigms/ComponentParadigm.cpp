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
        SAVANNA_ASSERT(pParadigmMemory != nullptr);
        SAVANNA_ASSERT(paradigmMemorySize > 0);

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

    // void Paradigm::UpdateParadigmLayout()
    // {
    //     // For each component in the paradigm, update the component layout
    //     // ComponentMetaData metaData = ComponentRegistry::AcquireComponentMetaDataFromKeyChain(m_ParadigmKeyChain);
    //     // size_t totalSize = metaData.GetTotalSize();
    //     // size_t numberOfComponents = metaData.GetNumberOfComponents();

    //     // void* pScratchMemory = malloc(m_ParadigmMemorySize);
    //     // memcpy(pScratchMemory, m_ParadigmMemory, m_ParadigmMemorySize);

    //     // for (size_t i = 0; i < numberOfComponents; i++)
    //     // {
    //     //     // TODO @DavidMohrhardt Need to store the previous component layout to properly copy the data
    //     //     // to the new layout.

    //     // }

    //     // ComponentRegistry::ReleaseComponentMetaDataFromKeyChain(metaData);
    // }

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

    ArraySlice<ComponentKey> Paradigm::GetKeyChain() const
    {
        return ArraySlice<ComponentKey>(m_ParadigmKeyChain, SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS);
    }
} // namespace Savanna::Entities

