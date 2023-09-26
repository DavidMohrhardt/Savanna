#include "GfxContext.h"

#include "Vk/SavannaVk2.h"

#if SAVANNA_WINDOWS
// #include "DX12/SavannaDX12.h"
#endif

namespace Savanna::Gfx
{
    GfxContext::GfxContext(const se_GfxContextCreateInfo_t* const pCreateInfo)
        : m_Allocator(pCreateInfo != nullptr
            ? pCreateInfo->m_Allocator
            : MemoryManager::GetAllocatorInterfaceForLabel(SE_MEMORY_LABEL_GRAPHICS))
    {
    }

    GfxContext::~GfxContext()
    {
        if (m_pDriver)
        {
            m_pDriver->Destroy();
            m_Allocator.Free(m_pDriver);
        }
    }

    se_GfxErrorCode_t GfxContext::CreateDriver(
        const se_GfxDriverCreateInfoList_t* const pCreateInfoList)
    {
        if (m_pDriver)
        {
            return kSavannaGfxErrorCodeGfxDriverAlreadyCreated;
        }

        if (!pCreateInfoList)
        {
            return kSavannaGfxErrorCodeInvalidArgument;
        }

        auto outResult = kSavannaGfxErrorCodeSuccess;
        outResult = kSavannaGfxErrorCodeUnknownError;
        for (int i = 0; i < pCreateInfoList->m_CreateInfoCount; ++i)
        {
            switch (pCreateInfoList->m_pDriverCreateInfos[i].m_RequestedBackendType)
            {
            case kSavannaGfxApiVulkan:
                outResult = Vk2::CreateDriver(
                    &pCreateInfoList->m_pDriverCreateInfos[i],
                    &m_pDriver,
                    pCreateInfoList->m_pUserData);
                break;

            // case kSavannaGfxExternalBackend:
            //     outResult = GenericGfxDriver::Create(
            //         pCreateInfo->m_pDriverCreateInfos[i],
            //         m_pDriver,
            //         pCreateInfo->m_pUserData);

            default:
                return kSavannaGfxErrorCodeUnsupportedGfxBackend;
            }
        }

        return outResult;
    }

    se_GfxSupportedBackend_t GfxContext::GetSupportedGfxBackends() const
    {
        return kSavannaSupportedGfxApiVulkan;
    }
} // namespace Savanna::Gfx
