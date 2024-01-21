#include "GfxContext.h"

extern seGfxErrorCode GetDriverInterfaceVulkan(seGfxDriverInterface& outDriverInterface);

namespace savanna::gfx
{
    GfxContext::GfxContext(const seGfxContextCreateInfo* const pCreateInfo)
        : m_Allocator(pCreateInfo != nullptr
            ? *pCreateInfo->m_pAllocatorInterface
            : MemoryManager::GetAllocatorInterfaceForAllocatorKind(kSavannaAllocatorKindPersistent))
        , m_Driver {pCreateInfo != nullptr
            ? *pCreateInfo->m_pAllocatorInterface
            : MemoryManager::GetAllocatorInterfaceForAllocatorKind(kSavannaAllocatorKindPersistent)}
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

    seGfxErrorCode GfxContext::CreateDriver(
        const seGfxDriverCreateInfoList* const pCreateInfoList)
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
            const seGfxDriverCreateInfo& driverCreateInfo = pCreateInfoList->m_pDriverCreateInfos[i];
            seGfxDriverInterface driverInterface{};
            switch (driverCreateInfo.m_RequestedBackendType)
            {
            case kSavannaGfxApiVulkan:
                outResult = GetDriverInterfaceVulkan(driverInterface);
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

    seGfxSupportedBackend GfxContext::GetSupportedGfxBackends() const
    {
        return kSavannaSupportedGfxApiVulkan;
    }
} // namespace savanna::Gfx
