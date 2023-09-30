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
            : MemoryManager::GetAllocatorInterfaceForLabel(k_SavannaMemoryLabelGfx))
        , m_pDriver(nullptr)
    {
    }

    GfxContext::~GfxContext()
    {
        if (m_pDriver != nullptr)
        {
            m_pDriver->Destroy();
            m_Allocator.Delete(m_pDriver);
        }
    }

    se_GfxErrorCode_t GfxContext::CreateDriver(
        const se_GfxDriverCreateInfoList_t* const pCreateInfoList)
    {
        if (m_pDriver != nullptr)
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
            const se_GfxDriverCreateInfo_t& driverCreateInfo = pCreateInfoList->m_pDriverCreateInfos[i];
            switch (driverCreateInfo.m_RequestedBackendType)
            {
            case kSavannaGfxApiVulkan:
                outResult = Vk2::AcquireDriver(
                    &driverCreateInfo,
                    &m_pDriver,
                    pCreateInfoList->m_pUserData,
                    m_Allocator.GetInterface());
                break;

            // case kSavannaGfxExternalBackend:
            //     outResult = GenericGfxDriver::Create(
            //         pCreateInfo->m_pDriverCreateInfos[i],
            //         m_pDriver,
            //         pCreateInfo->m_pUserData);

            default:
                SAVANNA_WARNING_LOG("Unknown backend type requested.");
                break;
            }

            if (outResult == kSavannaGfxErrorCodeSuccess)
            {
                return outResult;
            }
        }

        return kSavannaGfxErrorCodeUnableToCreateGfxDriver;
    }

    se_GfxSupportedBackend_t GfxContext::GetSupportedGfxBackends() const
    {
        return kSavannaSupportedGfxApiVulkan;
    }
} // namespace Savanna::Gfx
