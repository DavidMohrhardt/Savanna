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
        , m_Driver{}
    {
    }

    GfxContext::~GfxContext()
    {
        if (m_Driver.IsValid())
        {
            m_Driver.Destroy();
            m_Driver.ClearInterface();
        }
    }

    se_GfxErrorCode_t GfxContext::CreateDriver(
        const se_GfxDriverCreateInfoList_t* const pCreateInfoList)
    {
        if (m_Driver.IsValid())
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
            se_GfxDriverInterface_t driverInterface{};
            switch (driverCreateInfo.m_RequestedBackendType)
            {
            case kSavannaGfxApiVulkan:
                outResult = Vk2::GetDriverInterface(driverInterface);
                break;

            default:
                SAVANNA_WARNING_LOG("Unknown backend type requested.");
                break;
            }

            if (outResult == kSavannaGfxErrorCodeSuccess && driverInterface.m_pfnInitialize(driverCreateInfo) == kSavannaGfxErrorCodeSuccess)
            {
                m_Driver.SetInterface(driverInterface);
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
